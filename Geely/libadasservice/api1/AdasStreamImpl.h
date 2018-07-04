
#ifndef ANDROID_SERVERS_RVC_STREAM_IMPL_H
#define ANDROID_SERVERS_RVC_STREAM_IMPL_H

#include "AdasServiceImpl.h"
#include "IAdasClient.h"
#include "AdasDefine.h"
#include "NotifyClient.h"


namespace android
{
class AdasStreamImpl : public AdasServiceImpl::AdasStream
{
public:
    AdasStreamImpl();
    virtual ~AdasStreamImpl();

    virtual int showPDCView();
    virtual int hidePDCView();

    virtual int showGuidLineView();
    virtual int hideGuidLineView();

    virtual int disconnect();

    virtual int setPDCView(int pdcZone[] , int moduleFlag);
    virtual int setGuidLineView(int guidLineZone[] , int moduleFlag);
    virtual int setVideoView(int videoZone[] , int moduleFlag);
    virtual int setView3DPos(int view3DPos[] , int moduleFlag);

    virtual int showCameraVideo(int showCameraID);
    virtual int hideCameraVideo(int hideCameraID);

    virtual int enlargeSingleView(int cameraID);
    virtual int exitSingleView(int cameraID);

    virtual int show3DView();
    virtual int exitShow3DView();

    virtual int onKeyDown(int x, int y);
    virtual int onMove(int x, int y);
    virtual int onKeyUp(int x, int y);

    virtual int setCarColor(int color);

    virtual int init(int moduleFlag);
    virtual int cleanUp(int moduleFlag);

    virtual int onExitAllRVCView(int reason);
    virtual int onExitAllAVMView(int reason);

    virtual int getHWConfig();
    virtual int prepareState(int flag);
	
    void init();

private:
        // camera operation mode		
    sp<IAdasClient>                    m_pClient;
    bool                                        m_bStatus;

    NotifyClient* m_test;

};

}

#endif
