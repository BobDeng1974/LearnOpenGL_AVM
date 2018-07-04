
#define LOG_TAG "adas"

#include <ui/DisplayInfo.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <sys/time.h>
#include <log/log.h>

#include "OverlayWindow.h"

using namespace android;


OverlayWindow::OverlayWindow()
{	
    mDisplayInfo = new DisplayInfo;
    mComposerClient  = new SurfaceComposerClient;
    m_vViewList.clear();
}

void OverlayWindow::init()
{
    ALOGI("OverlayWindow | init");

    if(mComposerClient != NULL) {
        ALOGI("mComposerClient  create ok");
    }

    sp<IBinder> dtoken(SurfaceComposerClient::getBuiltInDisplay(
                    ISurfaceComposer::eDisplayIdMain));
    status_t status = SurfaceComposerClient::getDisplayInfo(dtoken, mDisplayInfo);

    ALOGI("mDisplayInfo->w = %d, mDisplayInfo->h = %d", mDisplayInfo->w, mDisplayInfo->h);
}

OverlayWindow::~OverlayWindow() 
{
    if(mDisplayInfo != NULL){
        delete mDisplayInfo;
        mDisplayInfo = NULL;
    }
}

sp<SurfaceControl> OverlayWindow::createViewSurface(viewInfo& viewInfo)
{
     ALOGI("OverlayWindow ::createViewSurface [%s]  width[%d]  height[%d]", 
                viewInfo.name, viewInfo.width, viewInfo.height);
     ALOGI("viewInfo.format  [%d]", viewInfo.format);
     m_pSurfaceControl = mComposerClient->createSurface(String8(viewInfo.name), viewInfo.width, 
                                                                                                  viewInfo.height, viewInfo.format, 0);
     // m_pSurfaceControl = mComposerClient->createSurface(String8("Guideline"),
     //        1069, 720, PIXEL_FORMAT_RGB_888, 0);
     ALOGI("m_pSurfaceControl  ok");
     if(m_pSurfaceControl != NULL) {
         if(m_pSurfaceControl->isValid()) {
            ALOGI("Surface Control  isValid ");
            return m_pSurfaceControl;
         }
     }

     return NULL;
 }

void OverlayWindow::registerSurfaceView(sp<SurfaceControl>& viewcontrol)
{
    ALOGI("OverlayWindow::register ok");
    m_vViewList.push_back(viewcontrol);
}

void OverlayWindow::startWindow()
{
    if(m_vViewList.size() > 0) {
        Vector<sp<SurfaceControl> >::iterator it, end;
        for (it = m_vViewList.begin(); it != m_vViewList.end(); ++it) {
            SurfaceComposerClient::openGlobalTransaction();
            (*it)->setAlpha(1);
            (*it)->show();
            SurfaceComposerClient::closeGlobalTransaction();
        }
    }    
}

void OverlayWindow::stopWindow()
{
     if(m_vViewList.size() > 0) {
        Vector<sp<SurfaceControl> >::iterator it, end;
        for (it = m_vViewList.begin(); it != m_vViewList.end(); ++it) {
            SurfaceComposerClient::openGlobalTransaction();
            (*it)->setAlpha(1);
            (*it)->hide();
            SurfaceComposerClient::closeGlobalTransaction();
        }
    }   
}


