
#define LOG_TAG "adas"

#include <sys/sysinfo.h>

#include "AdasStreamImpl.h"
#include "StreamImplCtrl.h"
#include "vns/VnsPropertyManager.h"
#include "a15_m4_ipc/A15M4Ipc.h"



namespace android {

static int getCallingPid() 
{
    return IPCThreadState::self()->getCallingPid();
}

AdasStreamImpl::AdasStreamImpl() 
    :AdasStream()
{
    ALOGI("AdasStreamImpl | init");
    BnAdasStream::onFirstRef();
    m_test = new  NotifyClient();
}

AdasStreamImpl::~AdasStreamImpl() 
{
   
}

// start preview mode
int AdasStreamImpl::showPDCView() 
{

    return 0;
}

// stop preview mode
int AdasStreamImpl::hidePDCView() 
{
    return 0;
}

int AdasStreamImpl::showCameraVideo(int showCameraID) 
{
    ALOGI("AdasStreamImpl::showCameraVideo");
    SurfaceComposerClient::openGlobalTransaction();
    StreamImplCtrl::Instance()->getPDCViewCtrl()->setLayer(0);
    StreamImplCtrl::Instance()->getPDCViewCtrl()->setAlpha(1);
    StreamImplCtrl::Instance()->getPDCViewCtrl()->show();
    StreamImplCtrl::Instance()->getGLViewCtrl()->setLayer(0);
    StreamImplCtrl::Instance()->getGLViewCtrl()->setAlpha(1);
    StreamImplCtrl::Instance()->getGLViewCtrl()->show();
    SurfaceComposerClient::closeGlobalTransaction();
    return 0;
}

// stop preview mode
int AdasStreamImpl::hideCameraVideo(int hideCameraID) 
{
    return 0;
}

int AdasStreamImpl::showGuidLineView()
{
    ALOGI("AdasStreamImpl::showGuideLine");
    SurfaceComposerClient::openGlobalTransaction();
    StreamImplCtrl::Instance()->getGLViewCtrl()->setAlpha(1);
    StreamImplCtrl::Instance()->getGLViewCtrl()->show();
    SurfaceComposerClient::closeGlobalTransaction();


    // m_test->NotifyVehicleSpeedEvent(30.12);
    // sleep(1);
  
    return 0;
}

int AdasStreamImpl::hideGuidLineView()
{
    ALOGI("AdasStreamImpl::hideGuideLine");
    // guidelinePause();
    SurfaceComposerClient::openGlobalTransaction();
    StreamImplCtrl::Instance()->getGLViewCtrl()->setAlpha(0);
    StreamImplCtrl::Instance()->getGLViewCtrl()->hide();
    SurfaceComposerClient::closeGlobalTransaction();
    return 0;
}

int AdasStreamImpl::disconnect()
{
    return 0;
}


int AdasStreamImpl::setPDCView(int pdcZone[] , int moduleFlag)
{
    return 0;
}

int AdasStreamImpl::setGuidLineView(int guidLineZone[] , int moduleFlag)
{
    return 0;
}

int AdasStreamImpl::setVideoView(int videoZone[] , int moduleFlag)
{
    return 0;
}

int AdasStreamImpl::setView3DPos(int view3DPos[] , int moduleFlag)
{
    return 0;
}

int AdasStreamImpl::enlargeSingleView(int cameraID)
{
   if(cameraID == 1){
        m_test->NotifyReverseGearEvent(1);
   }
   if(cameraID == 2){
        m_test->NotifyVehicleSpeedEvent(10.5);
        A15M4Ipc::getInstance()->sendIPUMsg(MSG_A15_REQUEST_REAR);
   }
   if(cameraID == 3){
         m_test->NotifyVehicleSpeedEvent(15.5);
   }
  if(cameraID == 4){
         m_test->NotifyVehicleSpeedEvent(20.5);
   }
   if(cameraID == 5){
         m_test->NotifyReverseGearEvent(0);
   }
   if(cameraID == 6){
        m_test->NotifyCameraErrorEvent(1, 1);
   }
   if(cameraID == 7){
        m_test->NotifyCameraErrorEvent(1, 0);
   }
   if(cameraID == 8){
        m_test->NotifyRadarErrorEvent(1, 1);
   }
   if(cameraID == 9){
        m_test->NotifyRadarErrorEvent(1, 0);
   }

   return 0;
}

int AdasStreamImpl::exitSingleView(int cameraID)
{
    return 0;
}

int AdasStreamImpl::show3DView()
{
    return 0;
}

int AdasStreamImpl::exitShow3DView()
{
    return 0;
}

int AdasStreamImpl::onKeyDown(int x, int y)
{
    return 0;
}

int AdasStreamImpl::onMove(int x, int y)
{
    return 0;
}

int AdasStreamImpl::onKeyUp(int x, int y)
{
    return 0;
}

int AdasStreamImpl::setCarColor(int color)
{
    return 0;
}

int AdasStreamImpl::init(int moduleFlag)
{
    return 0;
}

int AdasStreamImpl::cleanUp(int moduleFlag)
{
    return 0;
}

int AdasStreamImpl::onExitAllRVCView(int reason)
{
    SurfaceComposerClient::openGlobalTransaction();
    StreamImplCtrl::Instance()->getPDCViewCtrl()->setAlpha(0);
    StreamImplCtrl::Instance()->getPDCViewCtrl()->hide();
    StreamImplCtrl::Instance()->getGLViewCtrl()->setAlpha(0);
    StreamImplCtrl::Instance()->getGLViewCtrl()->hide();
    SurfaceComposerClient::closeGlobalTransaction();

    m_test->NotifyRVCExitEvent(reason);
    return 0;
}

int AdasStreamImpl::onExitAllAVMView(int reason)
{
    m_test->NotifyAVMExitEvent(reason);
    return 0;
}

int AdasStreamImpl::getHWConfig()
{
  return 0;
}

int AdasStreamImpl::prepareState(int flag)
{
    if(flag == 0){
       A15M4Ipc::getInstance()->sendIPUMsg(MSG_A15_REQUEST_REAR);
    }else if(flag == 1){
        ALOGI("AdasStreamImpl::prepareState flag false");             
    }
    return 0;
}




};
// namespace android
