#ifndef ADAS_NOTIFY_CLIENT_H
#define ADAS_NOTIFY_CLIENT_H

#include "NotifyClientBase.h"

namespace android
{
class NotifyClient : public NotifyClientBase
{
public:	
   /*Notify Rvc Stream  */
    NotifyClient();
    virtual ~NotifyClient();

public:
    status_t NotifyReverseGearEvent(int isEnable);
    status_t NotifyRVCExitEvent(int reason);
    status_t NotifyAVMExitEvent(int reason);
    status_t NotifyVehicleSpeedEvent(double speed);
    status_t NotifyCameraErrorEvent(int cameraID, int cameraState);
    status_t NotifyRadarErrorEvent(int radarID, int radarState);
private:
    // check whether the calling process matches mClientPid.

};

}

#endif
