
#define LOG_TAG "adas"

#include "AdasServiceImpl.h"
#include "IProtocolUtils.h"
#include "AdasNotifyCtrl.h"
#include "StreamImplCtrl.h"
#include "vns/VnsPropertyManager.h"
#include "a15_m4_ipc/A15M4Ipc.h"

namespace android {

static int getCallingPid() 
{
    return IPCThreadState::self()->getCallingPid();
}

static int getCallingUid() 
{
    return IPCThreadState::self()->getCallingUid();
}

AdasServiceImpl::AdasStream::AdasStream() 
{
}

AdasServiceImpl::AdasStream::~AdasStream() 
{
}

// ------------------
AdasServiceImpl::AdasServiceImpl()
    :m_bBootStatus(false)
    ,m_iStreamID(-1)
{
    ALOGI("AdasServiceImpl constructor");
    m_vMapAdasStream.clear();
    m_vMapAdasClient.clear();
}

void AdasServiceImpl::onFirstRef() 
{
    ALOGI("AdasService::onFirstRef");

    const sp<IServiceManager> sm = defaultServiceManager();

    while (sm->checkService(String16("SurfaceFlinger")) == NULL) {
        ALOGI("Waiting for SurfaceFlinger");
        usleep(10000);      
    }

    A15M4Ipc::getInstance()->initialize();
    StreamImplCtrl::Instance()->init();
    BnAdasService::onFirstRef();
    AdasNotifyCtrl::getInstance()->init();
    VnsPropertyManager::getInstance()->init();  
    if(isEarlyBoot()){
        ALOGI("early booting.....");
        // while(!StreamImplCtrl::Instance()->isReady()){
        //     ALOGI("waiting..PDC...........");
        //     usleep(1000);
        // }
        // sleep(2);
        //StreamImplCtrl::Instance()->test();
    } 



}

AdasServiceImpl::~AdasServiceImpl() 
{ 
    AdasNotifyCtrl::deInstance();
    ALOGI("-----------~AdasServiceImpl--------------");
}

status_t AdasServiceImpl::connect(const sp<IAdasClient>& adasClient,
                               int streamId, 
                               const String16& clientPackageName,
                               int clientUid, 
                               sp<IAdasStream>& stream )
{
    ALOGI("inside AdasServiceImpl::connect adas..........");
    //TODO: check for Adas  reconnect
    if(m_vMapAdasClient.indexOfKey(clientUid) < 0)
   {    
       m_vMapAdasClient.add(clientUid, adasClient);
       AdasNotifyCtrl::getInstance()->registerClient(adasClient);
       ALOGI("AdasServiceImpl::connect  | clientUid  = %d", clientUid);

       if(m_vMapAdasStream.indexOfKey(streamId) >= 0)
        {
            ALOGI("AdasServiceImpl::connect  | stream has exists!");
            m_vMapAdasStream.removeItem(streamId);     
        }

        ALOGI("AdasServiceImpl::connect  | streamId  = %d", streamId);
        stream = StreamImplCtrl::Instance()->getAdasStreamImpl(true);
        m_vMapAdasStream.add(streamId, stream);

        IInterface::asBinder(adasClient)->linkToDeath(this);
   }
   else{
        ALOGI("AdasServiceImpl::connect  | adasClient has exists!");
        IInterface::asBinder(adasClient)->linkToDeath(this);
        return ALREADY_EXISTS;
   }

   return OK;
}

status_t AdasServiceImpl::addListener(const sp<IAdasServiceListener>& listener) 
{
    ALOGI("%s: Add listener %p", __FUNCTION__, listener.get());
    Mutex::Autolock lock(mServiceLock);
    Vector<sp<IAdasServiceListener> >::iterator it, end;
    for (it = mListenerList.begin(); it != mListenerList.end(); ++it) {
        if (IInterface::asBinder((*it)) == IInterface::asBinder(listener)) {
            ALOGI("%s: Tried to add listener %p which was already subscribed", __FUNCTION__, listener.get());
            return ALREADY_EXISTS;
        }
    }
    mListenerList.push_back(listener);
    ALOGI("numberoflisteners = %d ", mListenerList.size());
    return OK;
}

status_t AdasServiceImpl::removeListener(const sp<IAdasServiceListener>& listener) 
{
    ALOGI("%s: Remove listener %p", __FUNCTION__, listener.get());
    Mutex::Autolock lock(mServiceLock);
    Vector<sp<IAdasServiceListener> >::iterator it;
    for (it = mListenerList.begin(); it != mListenerList.end(); ++it) {
        if (IInterface::asBinder((*it)) == IInterface::asBinder(listener)) {
            mListenerList.erase(it);
            return OK;
        }
    }
    ALOGW("%s: Tried to remove a listener %p which was not subscribed", __FUNCTION__, listener.get());
    return BAD_VALUE;
}

status_t AdasServiceImpl::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) 
{
    return BnAdasService::onTransact(code, data, reply, flags);
}

void AdasServiceImpl::binderDied(const wp<IBinder> &who) 
{
     sp<IBinder> ibinder = who.promote();
     ibinder->unlinkToDeath(this);
    if(!m_vMapAdasClient.isEmpty()){
        ALOGD("client has died: taking control back and waiting for client to connect back");
        for(int i = 0; i<m_vMapAdasClient.size(); i++){
            if(who == IInterface::asBinder(m_vMapAdasClient.valueAt(i))) {
                ALOGW("client binder death, uid:%d", m_vMapAdasClient.keyAt(i));
                m_vMapAdasClient.removeItemsAt(i);
            }
        }
    }
    else{
        ALOGD("adas client list is empty");
    }  
}

};
// namespace android
