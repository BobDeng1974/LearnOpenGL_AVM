#define LOG_TAG "adas"

#include "NotifyServerBase.h"
#include "AdasNotifyCtrl.h"

namespace android {

NotifyServerBase::NotifyServerBase()
{
    ALOGI("NotifyServerBase:NotifyServerBase ok");
    m_vClientList.clear();
}

NotifyServerBase::~NotifyServerBase() 
{

}

int NotifyServerBase::onSignal(const std::string &strData)
{

    return hanleMsg(strData);
}

void NotifyServerBase::registerClient(const sp<IAdasClient>& adasClient)
{
    NotifyServerBase::m_vClientList.push_back(adasClient);
}

}