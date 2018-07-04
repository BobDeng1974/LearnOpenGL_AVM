#ifndef ADAS_NOTIFY_SERVER_H
#define ADAS_NOTIFY_SERVER_H

#include <utils/RefBase.h>
#include "AdasDefine.h"
#include "IAdasClient.h"
#include "NotifyServerBase.h"


namespace android
{
class NotifyServer : public RefBase
		     , NotifyServerBase
{
public:	
   /*Notify Rvc Stream  */
    NotifyServer();
    virtual ~NotifyServer();
    virtual int hanleMsg(const std::string &msg);

public:
     void NotifyRearGearEvent(Json::Value& jsonParams);
     void NotifyRVCExitEvent(Json::Value& jsonParams);
     void NotifyAVMExitEvent(Json::Value& jsonParams);
     void NotifyVehicleSpeedEvent(Json::Value& jsonParams);
     void NotifyCameraErrorEvent(Json::Value& jsonParams);
     void NotifyRadarErrorEvent(Json::Value& jsonParams);
private:
};

}

#endif