#ifndef ADAS_VNS_PROPERTY_RECEIVER_H_
#define ADAS_VNS_PROPERTY_RECEIVER_H_

#include <utils/RefBase.h>
#include "AdasDefine.h"
#include "notify/NotifyClient.h"
#include "VnsPropertyDefine.h"
#include "IVehicleNetworkListener.h"
#include "VnsPropertyDefine.h"


namespace android
{
class VnsPropertyReceiver :  public BnVehicleNetworkListener
{
public:	
   /*property receiver  */
    VnsPropertyReceiver();
    virtual ~VnsPropertyReceiver();
    virtual void onEvents(sp<VehiclePropValueListHolder>& events);
    virtual void onHalError(int32_t errorCode, int32_t property, int32_t operation);
    virtual void onHalRestart(bool inMocking);

public:
    void handleEvent(int32_t& prop, vehicle_value_t& value);
    void parseErrorParam(vehicle_value_t& value);
    void updateRadarInfoParam(vehicle_value_t& value);
    void waitForEvents(nsecs_t reltime);
    bool waitForEvent(int32_t property, nsecs_t reltime);
    int getEventCount(int32_t property);
    int getHalRestartCount();
    void waitForHalRestart(nsecs_t reltime);
    void waitForHalError(nsecs_t reltime);
    bool isErrorMatching(int32_t errorCode, int32_t property, int32_t operation);

private:
     int getEventCountLocked(int32_t property);
     
private:

    Mutex mLock;
    Condition mCondition;
    KeyedVector<int32_t, int> mEventCounts;

    Mutex mHalRestartLock;
    Condition mHalRestartCondition;
    int mHalRestartCount;

    Mutex mHalErrorLock;
    Condition mHalErrorCondition;
    int32_t mErrorCode;
    int32_t mProperty;
    int32_t mOperation;

    NotifyClient* m_notifyClient;

};

}

#endif