
 #define LOG_TAG "adas"

#include <log/log.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include "IAdasService.h"
#include "IProtocolUtils.h"
#include "IAdasClient.h"
#include "AdasDefine.h"

namespace android {

class BpAdasClient: public BpInterface<IAdasClient> {
public:
    BpAdasClient(const sp<IBinder>& impl) : BpInterface<IAdasClient>(impl) 
    {
    }

    int onUpdateGearMark(int gearState) 
    {
        ALOGI("BpAdasClient::onUpdateGearMark");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(gearState);
        remote()->transact(BnAdasClient::ON_GEAR_MARK_EVENT, data,
			 &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onUpdateVehicleSpeed(double speed) 
    {
        ALOGI("BpAdasClient::onUpdateVehicleSpeed");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeDouble(speed);
        remote()->transact(BnAdasClient::ON_VEHICLE_SPEED_EVENT, data,
			 &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onUpdateRadarSignal(int radarLevel[]) 
    {
        ALOGI("BpAdasClient::onUpdateRadarSignal");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32Array(RADAR_NUM_MAX, radarLevel);
        remote()->transact(BnAdasClient::ON_RADAR_SIGNAL_LEVEL_EVENT, data,
			 &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onUpdateRadarSignal_(int radarID, int radarLevel) 
    {
        ALOGI("BpAdasClient::onUpdateRadarSignal_");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(radarID);
        data.writeInt32(radarLevel);
        remote()->transact(BnAdasClient::ON_RADAR_SIGNAL_EVENT, data,
                      &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onErrorOccured(int errorCode) 
    {
        ALOGI("BpAdasClient::onErrorOccured");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(errorCode);
        remote()->transact(BnAdasClient::ON_ERROR_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onExitCameraView(int cameraID) 
    {
        ALOGI("BpAdasClient::onExitCameraView");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(cameraID);
        remote()->transact(BnAdasClient::ON_EXIT_CAMERA_VIEW_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onExitPDCView()
    {
        ALOGI("BpAdasClient::onExitPDCView");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        remote()->transact(BnAdasClient::ON_EXIT_PDC_VIEW_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onExitGuideLineView()
    {
        ALOGI("BpAdasClient::onExitGuideLineView");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        remote()->transact(BnAdasClient::ON_EXIT_GUIDELINE_VIEW_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onExitAllRVCView(int reason)
    {
        ALOGI("BpAdasClient::onExitAllRVCView");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        ALOGI("1");
        data.writeInt32(reason);
        ALOGI("2");
        remote()->transact(BnAdasClient::ON_EXIT_ALL_RVC_VIEW_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        ALOGI("3");
        return 0;
    }

    int onExitAllAVMView(int reason)
    {
        ALOGI("BpAdasClient::onExitAllAVMView");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(reason);
        remote()->transact(BnAdasClient::ON_EXIT_ALL_AVM_VIEW_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onRadarFaultAlarm(int radarState[])
    {
        ALOGI("BpAdasClient::onRadarFaultAlarm");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32Array(RADAR_NUM_MAX, radarState);
        remote()->transact(BnAdasClient::ON_RADAR_STATE_FAULT_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onRadarFaultAlarm_(int radarID, int radarState)
    {
        ALOGI("BpAdasClient::onRadarFaultAlarm_");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(radarID);
        data.writeInt32(radarState);
        remote()->transact(BnAdasClient::ON_RADAR_FAULT_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onCameraFaultAlarm(int cameraState[] ) 
    {
        ALOGI("BpAdasClient::onCameraFaultAlarm");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32Array(CAMERA_NUM_MAX, cameraState);
        remote()->transact(BnAdasClient::ON_CAMERA_STATE_FAULT_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onCameraFaultAlarm_(int cameraID, int cameraState)
    {
        ALOGI("BpAdasClient::onCameraFaultAlarm_");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(cameraID);
        data.writeInt32(cameraState);
        remote()->transact(BnAdasClient::ON_CAMERA_FAULT_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onTurnSteeringColumn(int steerFlag)
    {
        ALOGI("BpAdasClient::onTurnSteeringColumn");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(steerFlag);
        remote()->transact(BnAdasClient::ON_TURN_STEERING_COLUMN_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onLeftDoorAlarm(int doorState)
    {
        ALOGI("BpAdasClient::onLeftDoorAlarm");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32(doorState);
        remote()->transact(BnAdasClient::ON_LEFT_DOOR_ALARM_EVNET, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int onSingleFrontViewSizeChange(int pos[])
    {
        ALOGI("BpAdasClient::onSingleFrontViewSizeChange");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32Array(CAMERA_NUM_MAX, pos);
        remote()->transact(BnAdasClient::ON_SINGLE_VIEW_CHANGE_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }

    int on3DViewSizeChange(int pos[])
    {
        ALOGI("BpAdasClient::on3DViewSizeChange");
        Parcel data, reply;
        data.writeInterfaceToken(IAdasClient::getInterfaceDescriptor());
        data.writeInt32Array(CAMERA_NUM_MAX, pos);
        remote()->transact(BnAdasClient::ON_3D_VIEW_CHANGE_EVENT, data,
             &reply, IBinder::FLAG_ONEWAY);
        return 0;
    }




};

IMPLEMENT_META_INTERFACE(AdasClient, "AdasClient");

// ----------------------------------------------------------------------

status_t BnAdasClient::onTransact(uint32_t code, const Parcel& data,
		Parcel* reply, uint32_t flags) 
{

	ALOGI("BnAdasClient::onTransact code = %d", code);
	 switch (code)
            {
                case ON_GEAR_MARK_EVENT: 
                {
                   ALOGI("BnAdasClient::onUpdateGearMark");
                   CHECK_INTERFACE(IAdasClient, data, reply);
                    int _arg0;
                    _arg0 = data.readInt32();
                    onUpdateGearMark(_arg0);
                    reply->writeNoException();
                    return OK;
                }
                case ON_VEHICLE_SPEED_EVENT: 
                {
                   ALOGI("BnAdasClient::onUpdateVehicleSpeed");
                   CHECK_INTERFACE(IAdasClient, data, reply);
                    double  _arg0;
                    _arg0 = data.readDouble();
                    onUpdateVehicleSpeed(_arg0);
                    reply->writeNoException();
                    return OK;
                }
                case ON_EXIT_ALL_RVC_VIEW_EVENT: 
                {
                   ALOGI("BnAdasClient::onExitAllRVCView");
                   CHECK_INTERFACE(IAdasClient, data, reply);
                    int  _arg0;
                    _arg0 = data.readInt32();
                    onExitAllRVCView(_arg0);
                    reply->writeNoException();
                    return OK;
                }
                case ON_EXIT_ALL_AVM_VIEW_EVENT: 
                {
                   ALOGI("BnAdasClient::onExitAllAVMView");
                   CHECK_INTERFACE(IAdasClient, data, reply);
                    int  _arg0;
                    _arg0 = data.readInt32();
                    onExitAllAVMView(_arg0);
                    reply->writeNoException();
                    return OK;
                }
                case ON_CAMERA_FAULT_EVENT:
                {
                    ALOGI("BnAdasClient::onCameraFaultAlarm_");
                   CHECK_INTERFACE(IAdasClient, data, reply);
                    int  _arg0;
                    int _arg1;
                    _arg0 = data.readInt32();
                    _arg1 = data.readInt32();
                    onCameraFaultAlarm_(_arg0, _arg1);
                    reply->writeNoException();
      
                    return OK;
                }
                case ON_RADAR_FAULT_EVENT:
                {
                    ALOGI("BnAdasClient::onRadarFaultAlarm_");
                    CHECK_INTERFACE(IAdasClient, data, reply);
                    int  _arg0;
                    int _arg1;
                    _arg0 = data.readInt32();
                    _arg1 = data.readInt32();
                    onRadarFaultAlarm_(_arg0, _arg1);
                    reply->writeNoException();
      
                    return OK;
                }
            }

	return BBinder::onTransact(code, data, reply, flags);
}

// ----------------------------------------------------------------------------

};
// namespace android
