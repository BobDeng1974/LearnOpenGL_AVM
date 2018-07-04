#ifndef ADAS_A15M4IPC_H_
#define ADAS_A15M4IPC_H_


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ti/ipc/Std.h>
#include <ti/ipc/Ipc.h>
#include <utils/Thread.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/transports/TransportRpmsg.h>

#include "AdasRunable.h"
#include "AdasDefine.h"
#include "notify/NotifyClient.h"


namespace android {

class A15M4Ipc : public AdasRunable
{
public:

    typedef struct SyncMsg 
    {
        MessageQ_MsgHeader header;
        UInt32 msg;
    } SyncMsg ;


enum MsgReq
{
    ReqWriteNewMsg,
    NoReqWriteMsg,
    UnKnown
};


    static A15M4Ipc* getInstance();
    virtual ~A15M4Ipc();

    virtual void update();
    virtual void init();
    status_t initialize();
    void sendIPUMsg(int msg);

private:
    A15M4Ipc();
    static A15M4Ipc* m_pInstance;    
    status_t connect();
    void wirteMsg();
    int getMsg(MessageQ_Msg& msg);
    void cleanUp();
    void handleMsg(int msgtype);

private:
    Mutex m_lock;
    bool mSync;
    nsecs_t m_timeout;

    UInt32 m_iHandle;
    MsgReq gStatus;
    int m_iMsg;

    MessageQ_Params    m_sMsgParams;
    MessageQ_Msg          m_sMsg;
    UInt32                                m_iStatus;
    MessageQ_QueueId    m_sQueueId;
    MessageQ_Handle       m_sMsgqHandle;
    char                     remoteQueueName[64];
    UInt32                 m_iMsgId;

    int m_iLastMsg;

    NotifyClient* m_notifyClient;
};
}

#endif