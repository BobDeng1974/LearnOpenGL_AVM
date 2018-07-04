#include "IAdasService.h"
#include "IAdasClient.h"
#include "IAdasStream.h"
#include <binder/BinderService.h>

namespace android {

class test_AdasVideoClient : 
				public BnAdasClient,
				public BinderService<test_AdasVideoClient>
{
   friend class BinderService<test_AdasVideoClient>;
public:
	test_AdasVideoClient();
	virtual ~test_AdasVideoClient();
	virtual int onUpdateGearMark(int gearState);
   	 virtual int onUpdateVehicleSpeed(double speed);
    	virtual int onUpdateRadarSignal(int radarLevel[] );
    	virtual int onUpdateRadarSignal_(int radarID, int radarLevel);
    	virtual int onErrorOccured(int errorCode);
    	virtual int onExitCameraView(int cameraID);
    	virtual int onExitPDCView();
    	virtual int onExitGuideLineView();
    	virtual int onExitAllRVCView(int reason);
    	virtual int onExitAllAVMView(int reason);
    	virtual int onRadarFaultAlarm(int radarState[] );
    	virtual int onRadarFaultAlarm_(int radarID, int radarState);
    	virtual int onCameraFaultAlarm(int cameraState[] );
    	virtual int onCameraFaultAlarm_(int cameraID, int cameraState);
    	virtual int onTurnSteeringColumn(int steerFlag);
    	virtual int onLeftDoorAlarm(int doorState);
    	virtual int onSingleFrontViewSizeChange(int pos[] );
    	virtual int on3DViewSizeChange(int pos[] );


protected:
	

private:

};



}