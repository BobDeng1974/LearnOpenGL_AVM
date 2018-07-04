
#ifndef  ADAS_IADASSERVICE_H
#define ADAS_IADASSERVICE_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <IAdasClient.h>
#include <IAdasServiceListener.h>
#include <IAdasStream.h>

namespace android
{

class IAdasService : public IInterface
{
public:
    enum
    {
        CONNECT = IBinder::FIRST_CALL_TRANSACTION,
        ADD_LISTENER,
        REMOVE_LISTENER
    };

    enum
    {
        USE_CALLING_UID = -1
    };

public:
    DECLARE_META_INTERFACE(AdasService);

    virtual status_t connect(const sp<IAdasClient>& adasClient,
                             int streamId,
                             const String16& clientPackageName,
                             int clientUid,
                             sp<IAdasStream>& stream ) = 0;

    virtual status_t addListener(const sp<IAdasServiceListener>& listener) = 0;

    virtual status_t removeListener(const sp<IAdasServiceListener>& listener) = 0;

};

// ----------------------------------------------------------------------------

class BnAdasService: public BnInterface<IAdasService>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif
