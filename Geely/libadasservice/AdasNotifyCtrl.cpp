#define LOG_TAG "adas"

#include <log/log.h>
#include "AdasNotifyCtrl.h"
#include "NotifyClient.h"
#include "NotifyServer.h"

namespace android{

AdasNotifyCtrl* AdasNotifyCtrl::m_pInstance = NULL;

AdasNotifyCtrl* AdasNotifyCtrl::getInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new AdasNotifyCtrl();
    }
    return m_pInstance;
}

void AdasNotifyCtrl::deInstance()
{	
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

AdasNotifyCtrl::AdasNotifyCtrl()
    : AdasRunable("AdasNotify")
    , m_sNotifyServerBase(NULL)
{
    ALOGI("AdasNotifyCtrl constructor");
    nMsgList.clear();
}

AdasNotifyCtrl::~AdasNotifyCtrl()
{
    quit();

    if(m_sNotifyServerBase != NULL)
    {
         delete m_sNotifyServerBase;
        m_sNotifyServerBase = NULL;
    }

    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }

    ALOGI("AdasNotifyCtrl destructor"); 
}

void AdasNotifyCtrl::pushMessage(eType type, std::string& parms)
{
   AutoMutex _l(m_lock);

   nMsgList.push_front(parms);

   ALOGI("AdasNotifyCtrl |  pushMessage ok"); 
}

void AdasNotifyCtrl::distroy()
{

}

void AdasNotifyCtrl::init()
{
    if(m_sNotifyServerBase == NULL){
        m_sNotifyServerBase = (NotifyServerBase*)new NotifyServer();
    }

    start();
}

void AdasNotifyCtrl::registerClient(const sp<IAdasClient>& adasClient)
{
      AutoMutex _l(m_lock); 
     if(m_sNotifyServerBase != NULL){
        m_sNotifyServerBase->registerClient(adasClient);
     }
}

void AdasNotifyCtrl::update()
{
     AutoMutex _l(m_lock);
    if(!nMsgList.empty())
    {
       std::string str = nMsgList.top();
       int ret = m_sNotifyServerBase->onSignal(str);
       if(ret == -1){
            ALOGI("AdasNotifyCtrl | update data error");
        }
       else{
            ALOGI("AdasNotifyCtrl | update ok!!!"); 
       }
       nMsgList.pop();
    }

    usleep(50000);
}

}

