
#define LOG_TAG "adas"

#include <cutils/log.h> /* log header file*/
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <gui/Surface.h>
#include "IAdasStream.h"
#include "IProtocolUtils.h"

namespace android
{

static void readIntArray(int length,  int* val, const Parcel& data) 
{
    if (length > 0) {
        for (int i=0; i<length; i++) {
            val[i] = data.readInt32();
        }
    } else {
            ALOGI("bad array lengths");
    }
}

class BpAdasStream: public BpInterface<IAdasStream>
{
public:
    BpAdasStream(const sp<IBinder>& impl)
        : BpInterface<IAdasStream>(impl)
    {
    }

int showPDCView() 
{
    ALOGI("showPDCView");
    Parcel data, reply;
    data.writeInterfaceToken(IAdasStream::getInterfaceDescriptor());
    remote()->transact(SHOW_PDC_VIEW, data, &reply);
    if (IProtocolUtils::readExceptionCode(reply)) return -1;
    return reply.readInt32();
}

// stop preview mode
int hidePDCView() 
{
    return 0;
}

int showCameraVideo(int showCameraID) 
{
    return 0;
}

int hideCameraVideo(int hideCameraID) 
{
    return 0;
}

int showGuidLineView()
{
    ALOGI("showGuidLineView");
    Parcel data, reply;
    data.writeInterfaceToken(IAdasStream::getInterfaceDescriptor());
    remote()->transact(SHOW_GUIDELINE_VIEW, data, &reply);
    if (IProtocolUtils::readExceptionCode(reply)) return -1;
    return reply.readInt32();
}

int hideGuidLineView()
{
    return 0;
}

int disconnect()
{
    return 0;
}


int setPDCView(int pdcZone[] , int moduleFlag)
{
    return 0;
}

int setGuidLineView(int guidLineZone[] , int moduleFlag)
{
    return 0;
}

int setVideoView(int videoZone[] , int moduleFlag)
{
    return 0;
}

int setView3DPos(int view3DPos[] , int moduleFlag)
{
    return 0;
}

int enlargeSingleView(int cameraID)
{
    ALOGI("showGuidLineView");
    Parcel data, reply;
    data.writeInterfaceToken(IAdasStream::getInterfaceDescriptor());
    data.writeInt32(cameraID); 
    remote()->transact(ENLARGE_SINGLE_VIEW, data, &reply);
    if (IProtocolUtils::readExceptionCode(reply)) return -1;
    return reply.readInt32();
}

int exitSingleView(int cameraID)
{
    return 0;
}

int show3DView()
{
    return 0;
}

int exitShow3DView()
{
    return 0;
}

int onKeyDown(int x, int y)
{
    return 0;
}

int onMove(int x, int y)
{
    return 0;
}

int onKeyUp(int x, int y)
{
    return 0;
}

int setCarColor(int color)
{
    return 0;
}

int init(int moduleFlag)
{
    return 0;
}

int cleanUp(int moduleFlag)
{
    return 0;
}

int onExitAllRVCView(int reason)
{
    return 0;
}

int onExitAllAVMView(int reason)
{
    return 0;
}

int getHWConfig()
{
    ALOGI("getHWConfig");
    Parcel data, reply;
    data.writeInterfaceToken(IAdasStream::getInterfaceDescriptor());
    remote()->transact(GET_HW_CONFIG, data, &reply);
    if (IProtocolUtils::readExceptionCode(reply)) return -1;
    return reply.readInt32();
}

int prepareState(int flag)
{
    ALOGI("prepareState");
    Parcel data, reply;
    data.writeInterfaceToken(IAdasStream::getInterfaceDescriptor());
    data.writeInt32(flag); 
    remote()->transact(PREPARE_STATE, data, &reply);
    if (IProtocolUtils::readExceptionCode(reply)) return -1;
    return reply.readInt32();
}


};

IMPLEMENT_META_INTERFACE(AdasStream, "AdasStream");

// ----------------------------------------------------------------------

status_t BnAdasStream::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code)
    {
    case SHOW_PDC_VIEW:
    {
        ALOGI("SHOW_PDC_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int status = showPDCView();
        if(status == 0)
          	reply->writeNoException();
        else
          	reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case HIDE_PDC_VIEW:
    {
        ALOGI("HIDE_PDC_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int status = hidePDCView();
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

     case SHOW_GUIDELINE_VIEW:
    {
        ALOGI("SHOW_GUIDELINE_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int status = showGuidLineView();
        if(status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case HIDE_GUIDELINE_VIEW:
    {
        ALOGI("HIDE_GUIDELINE_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int status = hideGuidLineView();
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;


    case DISCONNECT:
    {
        ALOGI("DISCONNECT");
        CHECK_INTERFACE(IAdasStream, data, reply);
        reply->writeNoException();
        reply->writeInt32(disconnect());
        return NO_ERROR;
    }
    break;

    case SET_PDC_VIEW:
    {
        ALOGI("SET_PDC_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int _arg1;
        _arg1 = data.readInt32();
        int _arg0[_arg1];
        readIntArray(_arg1, _arg0, data);
        int flag;
        flag = data.readInt32();
        ALOGI("IAdasStream::setPDCView  Array length  = %d, flag = %d", _arg1, flag);
        reply->writeNoException();
        reply->writeInt32(setPDCView(_arg0, flag));
        return NO_ERROR;
    }
    break;

    case SET_GUIDELINE_VIEW:
    {
        ALOGI("SET_GUIDELINE_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int _arg1;
        _arg1 = data.readInt32();
        int _arg0[_arg1];
        readIntArray(_arg1, _arg0, data);
        int flag;
        flag = data.readInt32();
        ALOGI("IAdasStream::setGuidLineView  Array length  = %d, flag = %d", _arg1, flag);
        reply->writeNoException();
        reply->writeInt32(setGuidLineView(_arg0, flag));
        return NO_ERROR;
    }
    break;

    case SET_VIDEO_VIEW:
    {
        ALOGI("SET_VIDEO_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int _arg1;
        _arg1 = data.readInt32();
        int _arg0[_arg1];
        readIntArray(_arg1, _arg0, data);
        int flag;
        flag = data.readInt32();
        ALOGI("IAdasStream::setGuidLineView  Array length  = %d, flag = %d", _arg1, flag);
        reply->writeNoException();
        reply->writeInt32(setVideoView(_arg0, flag));
        return NO_ERROR;
    }
    break;

    case SET_3D_POS_VIEW:
    {
        ALOGI("SET_3D_POS_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int _arg1;
        _arg1 = data.readInt32();
        int _arg0[_arg1];
        readIntArray(_arg1, _arg0, data);
        int flag;
        flag = data.readInt32();
        ALOGI("IAdasStream::setView3DPos  Array length  = %d, flag = %d", _arg1, flag);
        reply->writeNoException();
        reply->writeInt32(setView3DPos(_arg0, flag));
        return NO_ERROR;
    }
    break;

    case SHOW_CAMERA_VIDEO:
    {
        ALOGI("SHOW_CAMERA_VIDEO");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int cameraId = data.readInt32();
        ALOGI("IAdasStream::showCameraVideo  cameraid  = %d", cameraId);
        int status = showCameraVideo(cameraId);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case HIDE_CAMERA_VIDEO:
    {
        ALOGI("HIDE_CAMERA_VIDEO");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int cameraId = data.readInt32();
        ALOGI("IAdasStream::hideCameraVideo  cameraid  = %d", cameraId);
        int status = hideCameraVideo(cameraId);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case ENLARGE_SINGLE_VIEW:
    {
        ALOGI("ENLARGE_SINGLE_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int cameraId = data.readInt32();
        ALOGI("IAdasStream::enlargeSingleView  cameraid  = %d", cameraId);
        int status = enlargeSingleView(cameraId);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case EXIT_SINGLE_VIEW:
    {
        ALOGI("EXIT_SINGLE_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int cameraId = data.readInt32();
        ALOGI("IAdasStream::exitSingleView  cameraid  = %d", cameraId);
        int status = exitSingleView(cameraId);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case SHOW_3D_VIEW:
    {
        ALOGI("SHOW_3D_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int status = show3DView();
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case EXIT_3D_VIEW:
    {
        ALOGI("EXIT_3D_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int status = exitShow3DView();
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case ON_KEY_DOWN:
    {
        ALOGI("ON_KEY_DOWN");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int x = data.readInt32();
        int y = data.readInt32();
        ALOGI("IAdasStream::onKeyDown  x  = %d y = %d", x, y);
        int status = onKeyDown(x, y);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case ON_MOVE:
    {
        ALOGI("ON_MOVE");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int x = data.readInt32();
        int y = data.readInt32();
        ALOGI("IAdasStream::onMove  x  = %d y = %d", x, y);
        int status = onMove(x, y);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case ON_KEY_UP:
    {
        ALOGI("ON_KEY_UP");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int x = data.readInt32();
        int y = data.readInt32();
        ALOGI("IAdasStream::onKeyUp  x  = %d y = %d", x, y);
        int status = onKeyUp(x, y);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case SET_CAR_COLOR:
    {
        ALOGI("SET_CAR_COLOR");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int color = data.readInt32();
        ALOGI("IAdasStream::onKeyUp  color  = %d", color);
        int status = setCarColor(color);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case INIT:
    {
        ALOGI("INIT");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int flag = data.readInt32();
        ALOGI("IAdasStream::init  flag  = %d", flag);
        int status = init(flag);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case CLEAN_UP:
    {
        ALOGI("CLEAN_UP");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int flag = data.readInt32();
        ALOGI("IAdasStream::cleanUp  flag  = %d", flag);
        int status = cleanUp(flag);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case ON_EXIT_ALL_RVC_VIEW:
    {
        ALOGI("ON_EXIT_ALL_RVC_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int reason = data.readInt32();
        ALOGI("IAdasStream::onExitAllRVCView  reason  = %d", reason);
        int status = onExitAllRVCView(reason);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case ON_EXIT_ALL_AVM_VIEW:
    {
        ALOGI("ON_EXIT_ALL_AVM_VIEW");
        CHECK_INTERFACE(IAdasStream, data, reply);

        int reason = data.readInt32();
        ALOGI("IAdasStream::onExitAllAVMView  reason  = %d", reason);
        int status = onExitAllAVMView(reason);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;

    case GET_HW_CONFIG:
    {
       ALOGI("GET_HW_CONFIG");
        CHECK_INTERFACE(IAdasStream, data, reply);
        int ret = getHWConfig();
        reply->writeNoException();
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    case PREPARE_STATE:
    {
        ALOGI("PREPARE_STATE");
        CHECK_INTERFACE(IAdasStream, data, reply);

        int flag = data.readInt32();
        ALOGI("IAdasStream::PREPARE_STATE  flag  = %d", flag);
        int status = prepareState(flag);
        if (status == 0)
            reply->writeNoException();
        else
            reply->writeInt32(status);
        return NO_ERROR;
    }
    break;
    
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

// ----------------------------------------------------------------------------

}// namespace android
