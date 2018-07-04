
#ifndef ADAS_IADASRVCSTREAM_H
#define ADAS_IADASRVCSTREAM_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <utils/String8.h>


namespace android
{

class IAdasStream: public IInterface
{
    
public:

   enum
    { 
        SHOW_PDC_VIEW = IBinder::FIRST_CALL_TRANSACTION, //0
        HIDE_PDC_VIEW, //1

        SHOW_GUIDELINE_VIEW,//2,
        HIDE_GUIDELINE_VIEW,//3,

        DISCONNECT,//4,
        
        SET_PDC_VIEW,//5,
        SET_GUIDELINE_VIEW,//6,

        SET_VIDEO_VIEW,//7
        SET_3D_POS_VIEW,//8

        SHOW_CAMERA_VIDEO,//9
        HIDE_CAMERA_VIDEO,//10

        ENLARGE_SINGLE_VIEW,//11
        EXIT_SINGLE_VIEW,//12

        SHOW_3D_VIEW,//13
        EXIT_3D_VIEW,//14

        ON_KEY_DOWN,//15
        ON_MOVE,//16
        ON_KEY_UP,//17

        SET_CAR_COLOR,//18

        INIT,//19
        CLEAN_UP,//20

        ON_EXIT_ALL_RVC_VIEW,//21
        ON_EXIT_ALL_AVM_VIEW,//22

        GET_HW_CONFIG,
        PREPARE_STATE
    };

    enum
    {
        USE_CALLING_UID = -1
    };

    DECLARE_META_INTERFACE(AdasStream);

    virtual int showPDCView() = 0;
    virtual int hidePDCView() = 0;

    virtual int showGuidLineView() = 0;
    virtual int hideGuidLineView() = 0;

    virtual int disconnect() = 0;

    virtual int setPDCView(int pdcZone[] , int moduleFlag) = 0;
    virtual int setGuidLineView(int guidLineZone[] , int moduleFlag) = 0;
    virtual int setVideoView(int videoZone[] , int moduleFlag) = 0;
    virtual int setView3DPos(int view3DPos[] , int moduleFlag) = 0;

    virtual int showCameraVideo(int showCameraID) = 0;
    virtual int hideCameraVideo(int hideCameraID) = 0;

    virtual int enlargeSingleView(int cameraID) = 0;
    virtual int exitSingleView(int cameraID) = 0;

    virtual int show3DView() = 0;
    virtual int exitShow3DView() = 0;

    virtual int onKeyDown(int x, int y) = 0;
    virtual int onMove(int x, int y) = 0;
    virtual int onKeyUp(int x, int y) = 0;

    virtual int setCarColor(int color) = 0;

    virtual int init(int moduleFlag) = 0;
    virtual int cleanUp(int moduleFlag) = 0;

    virtual int onExitAllRVCView(int reason) = 0;
    virtual int onExitAllAVMView(int reason) = 0;

    virtual int getHWConfig() = 0;
    virtual int prepareState(int flag) = 0;

};

// ----------------------------------------------------------------------------

class BnAdasStream: public BnInterface<IAdasStream>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif
