
#define LOG_TAG "adas"

#include "VnsPropertyReceiver.h"
#include "VnsPropertyManager.h"
#include "StreamImplCtrl.h"

namespace android
{

VnsPropertyReceiver::VnsPropertyReceiver()
{
    m_notifyClient = new  NotifyClient();
}

VnsPropertyReceiver::~VnsPropertyReceiver()
{
    if(m_notifyClient != NULL) {
        delete m_notifyClient;
        m_notifyClient = NULL;
    }
}

void VnsPropertyReceiver::handleEvent(int32_t& prop, vehicle_value_t& value)
{
    if(prop == VEHICLE_PROPERTY_BCM_REVERSEGEARINFO) {
        ALOGI("VnsPropertyReceiver  prop  = NotifyReverseGearEvent value = %d", (int)value.boolean_value);
        m_notifyClient->NotifyReverseGearEvent((int)value.boolean_value);
    }
    else if(prop == VEHICLE_PROPERTY_IPK_AVERAGEVEHICLESPEED) {
        ALOGI("VnsPropertyReceiver  prop  = NotifyVehicleSpeedEvent value = %f", (double)value.int32_value);
        m_notifyClient->NotifyVehicleSpeedEvent((double)value.int32_value);
    }
    else if(prop == VEHICLE_PROPERTY_SAS_STEERWHEELANGLE) {
        ALOGI("VnsPropertyReceiver  prop  = updateSteerWheelAngle value = %d", (int)value.int32_value);
        StreamImplCtrl::Instance()->updateSteerWheelAngle((int)value.int32_value);
    }
    else if(prop == VEHICLE_PROPERTY_SENSOR_ADAS_STATUS_REQUEST) {

    }
    else if(prop == VEHICLE_PROPERTY_SENSOR_RADAR_ERROR_STATUS_NOTIFY) {
        ALOGI("VnsPropertyReceiver  prop  = NotifyRadarErrorEvent ");
        parseErrorParam(value);
    }
    else if(prop == VEHICLE_PROPERTY_SENSOR_RADAR_SENSOR_DATA_NOTIFY) {
        ALOGI("VnsPropertyReceiver  prop  = RadarDataEvent");
        updateRadarInfoParam(value);
    }
}

void VnsPropertyReceiver::parseErrorParam(vehicle_value_t& param)
{
     ScopedVehiclePropValue radarInfovalue;
     radarInfovalue.value.value = param;
     int len = radarInfovalue.value.value.bytes_value.len;
     uint8_t* radar = radarInfovalue.value.value.bytes_value.data;
     switch(radar[1] & 0x0f){
        case radarError_RearRight:
            {
                ALOGI("VnsPropertyReceiver  radarError_RearRight");
            }
        break;

        case radarError_RearMiddleLeft:
            {
                ALOGI("VnsPropertyReceiver  radarError_RearMiddleLeft");
            }
        break;

        case radarError_RearLeft:
            {
                ALOGI("VnsPropertyReceiver  radarError_RearLeft");
            }
        break;

        case radarError_RearRightAndRearMiddleLeft:
            {
                ALOGI("VnsPropertyReceiver  radarError_RearRightAndRearMiddleLeft");
            }
        break;

        case radarError_RearLeftAndRearMiddleLeft:
            {
                ALOGI("VnsPropertyReceiver  radarError_RearLeftAndRearMiddleLeft");
            }
        break;

        case radarError_RearLeftAndRearRight:
            {
                ALOGI("VnsPropertyReceiver  radarError_RearLeftAndRearRight");
            }
        break;

        case radarError_ALL:
            {
                ALOGI("VnsPropertyReceiver  radarError_ALL");
            }
        break;

        default:
               ALOGI("VnsPropertyReceiver  radar OK");
     }
     return;
}

void VnsPropertyReceiver::updateRadarInfoParam(vehicle_value_t& param)
{
     ALOGI("VnsPropertyReceiver | updateRadarInfoParam");
     ScopedVehiclePropValue radarInfovalue;
     radarInfovalue.value.value = param;
     int len = radarInfovalue.value.value.bytes_value.len;
     uint8_t* radar = radarInfovalue.value.value.bytes_value.data;
     radarType RvcRadarInfo[RVC_RADAR_NUM] = {
            {4, radar[5]&0x01 , radar[0]},  //Distance for Rear Right Sensor 
            {6, radar[5]&0x04 , radar[2]},  //Distance for Rear Middle Left Senso
            {7, radar[5]&0x08 , radar[3]},   //Distance for Rear Left Sensor 
        };
    ALOGI("VnsPropertyReceiver | updateRadarInfoParam len[%d] radar[0][%d] radar[2][%d]" , len, radar[0], radar[2]);
    StreamImplCtrl::Instance()->updateRadar(RvcRadarInfo);
}

void VnsPropertyReceiver::onEvents(sp<VehiclePropValueListHolder>& events)
{
    ALOGI("VnsPropertyReceiver | onEvents start");
    String8 msg("events ");
    Mutex::Autolock autolock(mLock);
    for (auto& e : events->getList()) {
            ssize_t index = mEventCounts.indexOfKey(e->prop);
            if (index < 0) {
                ALOGI("Event index < 0 ");
                mEventCounts.add(e->prop, 1); // 1st event
                msg.appendFormat("0x%x:%d ", e->prop, 1);
            } else {
                ALOGI("Event index[%d]", index);
                int count = mEventCounts.valueAt(index);
                count++;
                mEventCounts.replaceValueAt(index, count);
                msg.appendFormat("0x%x:%d ", e->prop, count);
            }
        ALOGI("VnsPropertyReceiver | onEvents[%s]  value[%d]", msg.string(), e->value);

        handleEvent(e->prop, e->value);
    }
}

 void VnsPropertyReceiver::onHalError(int32_t errorCode, int32_t property, int32_t operation) 
 {
    Mutex::Autolock autolock(mHalErrorLock);
    mErrorCode = errorCode;
    mProperty = property;
    mOperation = operation;
}

void VnsPropertyReceiver::onHalRestart(bool /*inMocking*/) 
{
    Mutex::Autolock autolock(mHalRestartLock);
    mHalRestartCount++;
}

// void VnsPropertyReceiver::waitForEvents(nsecs_t reltime)
// {
//      Mutex::Autolock autolock(mLock);
//     mCondition.waitRelative(mLock, reltime);
// }

// bool VnsPropertyReceiver::waitForEvent(int32_t property, nsecs_t reltime)
// {
//     Mutex::Autolock autolock(mLock);
//     int startCount = getEventCountLocked(property);
//     int currentCount = startCount;
//     int64_t now = android::elapsedRealtimeNano();
//     int64_t endTime = now + reltime;
//     while ((startCount == currentCount) && (now < endTime)) {
//         mCondition.waitRelative(mLock, endTime - now);
//         currentCount = getEventCountLocked(property);
//         now = android::elapsedRealtimeNano();
//     }
//     return (startCount != currentCount);
// }

// int VnsPropertyReceiver::getEventCount(int32_t property)
// {
//     Mutex::Autolock autolock(mLock);
//     return getEventCountLocked(property);
// }


// int VnsPropertyReceiver::getHalRestartCount()
// {
//      Mutex::Autolock autolock(mHalRestartLock);
//      return mHalRestartCount;
// }

// void VnsPropertyReceiver::waitForHalRestart(nsecs_t reltime) 
// {
//     Mutex::Autolock autolock(mHalRestartLock);
//     mHalRestartCondition.waitRelative(mHalRestartLock, reltime);
// }

// void VnsPropertyReceiver::waitForHalError(nsecs_t reltime) 
// {
//     Mutex::Autolock autolock(mHalErrorLock);
//     mHalErrorCondition.waitRelative(mHalErrorLock, reltime);
// }

// bool VnsPropertyReceiver::isErrorMatching(int32_t errorCode, int32_t property, int32_t operation) 
// {
//     Mutex::Autolock autolock(mHalErrorLock);
//     return mErrorCode == errorCode && mProperty == property && mOperation == operation;
// }

//  int VnsPropertyReceiver::getEventCountLocked(int32_t property) 
//  {
//         ssize_t index = mEventCounts.indexOfKey(property);
//         if (index < 0) {
//             return 0;
//         } else {
//             return mEventCounts.valueAt(index);
//         }
//  }

}