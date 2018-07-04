#include "test_AdasVideoClient.h"
#include <log/log.h>

#define LOG_TAG "adastest"

using namespace android;

test_AdasVideoClient::test_AdasVideoClient()
{

}

test_AdasVideoClient::~test_AdasVideoClient()
{

}

int test_AdasVideoClient::onUpdateGearMark(int gearState)
{
   ALOGI("onUpdateGearMark |  OK  gearState[%d]", gearState);    
  return 0;
}
  int test_AdasVideoClient::onUpdateVehicleSpeed(double speed)
  {
  ALOGI("onUpdateVehicleSpeed |  OK  speed[%d]", speed);  
    return 0;
  }
  int test_AdasVideoClient::onUpdateRadarSignal(int radarLevel[] )
  {
  	return 0;
  }
  int test_AdasVideoClient::onUpdateRadarSignal_(int radarID, int radarLevel)
  {
  	return 0;
  } 
  int test_AdasVideoClient::onErrorOccured(int errorCode)
  {
  	return 0;
  }
  int test_AdasVideoClient::onExitCameraView(int cameraID)
  {
  	return 0;
  }
  int test_AdasVideoClient::onExitPDCView()
  {
  	return 0;
  }
  int test_AdasVideoClient::onExitGuideLineView()
  {
  	return 0;
  }
  int test_AdasVideoClient::onExitAllRVCView(int reason)
  {
  ALOGI("onExitAllRVCView |  OK  ");  
    return 0;
  }
  int test_AdasVideoClient::onExitAllAVMView(int reason)
  {
  	return 0;
  }
  int test_AdasVideoClient::onRadarFaultAlarm(int radarState[] )
  {
  	return 0;
  }
  int test_AdasVideoClient::onRadarFaultAlarm_(int radarID, int radarState)
  {
  	return 0;
  }
  int test_AdasVideoClient::onCameraFaultAlarm(int cameraState[] )
  {
  	return 0;
  }
  int test_AdasVideoClient::onCameraFaultAlarm_(int cameraID, int cameraState)
  {
  	return 0;
  }
  int test_AdasVideoClient::onTurnSteeringColumn(int steerFlag)
  {
  	return 0;
  }
  int test_AdasVideoClient::onLeftDoorAlarm(int doorState)
  {
  	return 0;
  }
  int test_AdasVideoClient::onSingleFrontViewSizeChange(int pos[] )
  {
  	return 0;
  }
  int test_AdasVideoClient::on3DViewSizeChange(int pos[] )
  {
  	return 0;
  }