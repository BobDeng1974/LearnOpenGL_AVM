
#ifndef  ADAS_IADASCLIENT_H
#define ADAS_IADASCLIENT_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <utils/Timers.h>

namespace android
{

class IAdasClient: public IInterface
{
public:
    enum
    {
        ON_GEAR_MARK_EVENT = IBinder::FIRST_CALL_TRANSACTION,

        ON_VEHICLE_SPEED_EVENT,

        ON_RADAR_SIGNAL_LEVEL_EVENT,
        ON_RADAR_SIGNAL_EVENT,

        ON_ERROR_EVENT,

        ON_EXIT_CAMERA_VIEW_EVENT,

        ON_EXIT_PDC_VIEW_EVENT,

        ON_EXIT_GUIDELINE_VIEW_EVENT,

        ON_EXIT_ALL_RVC_VIEW_EVENT,
        ON_EXIT_ALL_AVM_VIEW_EVENT,

        ON_RADAR_STATE_FAULT_EVENT,
        ON_RADAR_FAULT_EVENT,

        ON_CAMERA_STATE_FAULT_EVENT,
        ON_CAMERA_FAULT_EVENT,

        ON_TURN_STEERING_COLUMN_EVENT,

        ON_LEFT_DOOR_ALARM_EVNET,

        ON_SINGLE_VIEW_CHANGE_EVENT,

        ON_3D_VIEW_CHANGE_EVENT
    };

    DECLARE_META_INTERFACE(AdasClient);

    virtual int onUpdateGearMark(int gearState) = 0;
    virtual int onUpdateVehicleSpeed(double speed) = 0;
    virtual int onUpdateRadarSignal(int radarLevel[] ) = 0;
    virtual int onUpdateRadarSignal_(int radarID, int radarLevel) = 0;
    virtual int onErrorOccured(int errorCode) = 0;
    virtual int onExitCameraView(int cameraID) = 0;
    virtual int onExitPDCView() = 0;
    virtual int onExitGuideLineView() = 0;
    virtual int onExitAllRVCView(int reason) = 0;
    virtual int onExitAllAVMView(int reason) = 0;
    virtual int onRadarFaultAlarm(int radarState[] ) = 0;
    virtual int onRadarFaultAlarm_(int radarID, int radarState) = 0;
    virtual int onCameraFaultAlarm(int cameraState[] ) = 0;
    virtual int onCameraFaultAlarm_(int cameraID, int cameraState) = 0;
    virtual int onTurnSteeringColumn(int steerFlag) = 0;
    virtual int onLeftDoorAlarm(int doorState) = 0;
    virtual int onSingleFrontViewSizeChange(int pos[] ) = 0;
    virtual int on3DViewSizeChange(int pos[] ) = 0;
};

// ----------------------------------------------------------------------------

class BnAdasClient: public BnInterface<IAdasClient>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif
