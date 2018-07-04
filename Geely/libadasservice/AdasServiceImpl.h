
#ifndef ADAS_SERVICE_IMPL_H
#define ADAS_SERVICE_IMPL_H

#include <utils/Vector.h>
#include <binder/BinderService.h>
#include <IAdasService.h>
#include <IAdasStream.h>
#include <IAdasClient.h>
#include "AdasDefine.h"


namespace android
{

class AdasServiceImpl :
    public BinderService<AdasServiceImpl>,
    public BnAdasService,
    public IBinder::DeathRecipient
{
    friend class BinderService<AdasServiceImpl>;
public:

    static char const* getServiceName()
    {
        return "AdasService";
    }    

    AdasServiceImpl();
    virtual   ~AdasServiceImpl();

    
    virtual status_t connect(const sp<IAdasClient>& adasClient,
                               int streamId, 
                               const String16& clientPackageName,
                               int clientUid, 
                               sp<IAdasStream>& stream );


    virtual status_t addListener(const sp<IAdasServiceListener>& listener);
    virtual status_t removeListener(const sp<IAdasServiceListener>& listener);

    // BnAdasVideoService
    virtual status_t    onTransact(uint32_t code, const Parcel& data,
                                   Parcel* reply, uint32_t flags);

    class AdasStream : public BnAdasStream
    {
    public:
        AdasStream();
        virtual ~AdasStream();

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

    protected :
        pid_t  mClientPid;
    };
	
private:
    virtual void onFirstRef();
    bool    isEarlyBoot() {return true;};

    status_t              validateConnect(int cameraId, int& clientUid) const;
    virtual void        binderDied(const wp<IBinder> &who);

    Mutex               mServiceLock;
    bool    m_bBootStatus;
	
    Vector<sp<IAdasServiceListener> >   mListenerList;
    KeyedVector<int, sp<IAdasStream> > m_vMapAdasStream;
    KeyedVector<int, sp<IAdasClient> > m_vMapAdasClient;
    sp<IAdasClient> m_pAdasClient;
    int m_iStreamID;
};

} // namespace android

#endif
