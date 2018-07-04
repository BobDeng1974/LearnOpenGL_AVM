#define LOG_TAG "adas"

#include "NotifyClientBase.h"
#include "AdasNotifyCtrl.h"

namespace android {

NotifyClientBase::NotifyClientBase()
{
    ALOGI("NotifyClientBase:NotifyClientBase ok");
}

NotifyClientBase::~NotifyClientBase() 
{

}

void NotifyClientBase::pushMessage(std::string& parm)
{
    ALOGI("4");
    AdasNotifyCtrl::getInstance()->pushMessage(eType_RVC, parm);
}

}

