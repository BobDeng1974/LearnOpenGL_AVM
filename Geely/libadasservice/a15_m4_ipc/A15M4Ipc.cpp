#define LOG_TAG "adas"

#include <log/log.h>
/* IPC Headers */
#include "A15M4Ipc.h"

using namespace android;

#define AS2INT(x) x=(x&~(0xffffff00))-0x30
#define HEAPID              0u
#define SLAVE_MESSAGEQNAME  "SLAVE"
#define MPU_MESSAGEQNAME    "HOST"

#define PROC_ID_DFLT        1     /* Host is zero, remote cores start at 1 */
#define NUM_LOOPS_DFLT   100

A15M4Ipc* A15M4Ipc::m_pInstance = NULL;


A15M4Ipc* A15M4Ipc::getInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new A15M4Ipc();
    }
    return m_pInstance;
}

A15M4Ipc::A15M4Ipc() 
    : AdasRunable("A15M4Ipc")
    , mSync(false)
    , m_iHandle(0)
    , gStatus(UnKnown)
    , m_iStatus(0)
    , m_sMsg(NULL)
    , m_sQueueId(MessageQ_INVALIDMESSAGEQ)
    , m_iMsgId(0)
    , m_iMsg(-1)
    , m_iLastMsg(-1)
{ 
    memset(remoteQueueName, 0, sizeof(char)*64);
     m_notifyClient = new  NotifyClient();
}

A15M4Ipc::~A15M4Ipc() 
{ 
    cleanUp();
    Ipc_stop();

    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void A15M4Ipc::init()
{
    ALOGI("Entered MessageQApp_execute");
    /*init messageQ */
    MessageQ_Params_init(&m_sMsgParams);
    /*create messageQ */
    m_sMsgqHandle = MessageQ_create(MPU_MESSAGEQNAME, &m_sMsgParams);
    ALOGI("********** MessageQ_create  OK");
    if (m_sMsgqHandle == NULL) {
        ALOGI("Error in MessageQ_create");
        return;
    }else{
        ALOGI("Local MessageQId: 0x%x", MessageQ_getQueueId(m_sMsgqHandle));
    }
    /*format messageQ name*/
    sprintf(remoteQueueName, "%s_%s", SLAVE_MESSAGEQNAME, MultiProc_getName(PROC_ID_DFLT));
    /*open messageQ */
    do {
        ALOGI("********** MessageQ_open    -----ing");
        m_iStatus = MessageQ_open(remoteQueueName, &m_sQueueId);
        usleep (100000);
     } while (m_iStatus == MessageQ_E_NOTFOUND);
    ALOGI("********** MessageQ_open  OK");
    if (m_iStatus < 0) {
        cleanUp();
        ALOGI("Error in MessageQ_open [%d]", m_iStatus);
        return;
    }
    /*alloc message buff*/
    m_sMsg = MessageQ_alloc(HEAPID, sizeof(SyncMsg));
    if (m_sMsg == NULL) {
        ALOGI("Error in MessageQ_alloc");
        MessageQ_close(&m_sQueueId);
        cleanUp();
        return;
    }

    ALOGI("********** MessageQ_alloc  OK");

    if(connect() == BAD_VALUE){
        ALOGI("A15M4Ipc::connect failed!");
        return;
    }

    ALOGI("A15M4Ipc::connect OK!");
}

void A15M4Ipc::cleanUp()
{
    m_iStatus = MessageQ_delete(&m_sMsgqHandle);
    if (m_iStatus < 0) {
        ALOGI("Error in MessageQ_delete [%d]", m_iStatus);
    }
}

status_t A15M4Ipc::initialize()
{
    int status = 0;
    Ipc_transportConfig(&TransportRpmsg_Factory);
    status = Ipc_start();
    if (status < 0) {
        ALOGI("Error: Ipc_start failed, error=%d\n", status);
        return BAD_VALUE;
    }
    start();
    return OK;
}

int A15M4Ipc::getMsg(MessageQ_Msg& msg)
{
    status_t ret;
    return 0;
}

void A15M4Ipc::wirteMsg()
{
    MessageQ_setReplyQueue(m_sMsgqHandle, m_sMsg);
    ((SyncMsg *)m_sMsg)->msg = m_iMsg;
    int status = MessageQ_put(m_sQueueId, m_sMsg); 
    if (status < 0) {
        ALOGI("wirteMsg | Error in MessageQ_put [%d]\n", status);
        MessageQ_free(m_sMsg);
        return;
    }else{
        ALOGI("A15M4Ipc::wirteMsg [%d] ok", m_iMsg);
    }
}

void A15M4Ipc::sendIPUMsg(int msg)
{
   AutoMutex _l(m_lock);
   m_iMsg = msg;
   gStatus = ReqWriteNewMsg;
}


status_t A15M4Ipc::connect()
{
    ALOGI("********** connect  -----ing");
    if(m_sMsgqHandle == NULL || m_sMsg == NULL) {
        ALOGI("Error in connect | m_sMsgqHandle == NULL || m_sMsg == NULL");
        MessageQ_close(&m_sQueueId);
        cleanUp();
        return BAD_VALUE;
    }
    ALOGI("********** MessageQ_setReplyQueue  -----ing");
    MessageQ_setReplyQueue(m_sMsgqHandle, m_sMsg);
    ALOGI("********** MessageQ_setReplyQueue  ok");
    ((SyncMsg *)m_sMsg)->msg = MSG_A15_INIT_OK;
    ALOGI("********** MessageQ_put  -----ing");
    int status = MessageQ_put(m_sQueueId, m_sMsg);   
    if (status < 0) {
            ALOGI("Error in MessageQ_put [%d]", status);
            MessageQ_free(m_sMsg);
            return BAD_VALUE;
    }
    ALOGI("A15 m4 first handshake ok....");

    while(1) {
         ALOGI("A15 m4 second handshake ------ing");
        MessageQ_setReplyQueue(m_sMsgqHandle, m_sMsg);
        status = MessageQ_get(m_sMsgqHandle, &m_sMsg, MessageQ_FOREVER);
        if (status < 0) {
            ALOGI("Error in MessageQ_get [%d], waiting for confirm....", status);
            continue;
        }
        else{
             if( ((SyncMsg *)m_sMsg)->msg== MSG_IPU_CONFIRM_OK ) {
                ALOGI("A15 m4 second handshake confirm ok....");
                break;
             }
             else{
                ALOGI("wait for A15 m4 second handshake confirm ...");
                continue;
             }
        }
        usleep(100000);
    }

    MessageQ_setReplyQueue(m_sMsgqHandle, m_sMsg);

    ((SyncMsg *)m_sMsg)->msg = MSG_A15_READY_FOR_REQUEST;
    status = MessageQ_put(m_sQueueId, m_sMsg);   
    if (status < 0) {
            ALOGI("Error in third MessageQ_put [%d]", status);
            MessageQ_free(m_sMsg);
            return BAD_VALUE;
    }

    ALOGI("A15 m4 third handshake confirm ok....");

    return OK;
}

void A15M4Ipc::handleMsg(int msgtype)
{
    ALOGI("A15M4Ipc::handleMsg [%d]", msgtype);
    switch(msgtype){
        case MSG_REAR_DATA_REVERS:
            ALOGI("MSG_REAR_DATA_REVERS");
            m_notifyClient->NotifyReverseGearEvent(1);
        break;
        case MSG_REAR_DATA_NO_REVERS:
            ALOGI("MSG_REAR_DATA_NO_REVERS");
            m_notifyClient->NotifyReverseGearEvent(0);
        break;
        default:
            ALOGI("msg ignore!");
        return;
    }
}

void A15M4Ipc::update()
{
    int msg = -1;
    if(gStatus == ReqWriteNewMsg) {
            wirteMsg();
            gStatus = NoReqWriteMsg;
    }
    msg = getMsg(m_sMsg);
     
    if(msg != m_iLastMsg) {
        m_iLastMsg = msg;
        handleMsg(msg);
    }
    usleep(100000);   
}

