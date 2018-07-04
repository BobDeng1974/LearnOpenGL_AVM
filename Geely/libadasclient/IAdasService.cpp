
#define LOG_TAG "adas"

#include <log/log.h> /* log header file*/
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <IAdasService.h>
#include <IProtocolUtils.h>

namespace android
{

class BpAdasService: public BpInterface<IAdasService>
{
public:
    BpAdasService(const sp<IBinder>& impl)
        : BpInterface<IAdasService>(impl)
    {
    }

    // connect to Adas video service
    virtual status_t connect(const sp<IAdasClient>& adasClient, int streamId,
                             const String16 &clientPackageName, int clientUid,
                             /*out*/
                             sp<IAdasStream>& device)
    {
        Parcel data, reply;
        ALOGI("BpAdasService connect adas");
        data.writeInterfaceToken(IAdasService::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(adasClient));
        data.writeInt32(streamId);
        data.writeString16(clientPackageName);
        data.writeInt32(clientUid);
        if( streamId < 0 )
        {
            device = NULL;
            return BAD_VALUE;
        }   
        ALOGI("inside connect rvc");
        remote()->transact(BnAdasService::CONNECT, data, &reply);

        // if (IProtocolUtils::readExceptionCode(reply)) return -EPROTO;
        // status_t status = reply.readInt32();

        device = interface_cast<IAdasStream>(reply.readStrongBinder());
        ALOGI("got IV********device = %d", device.get());
        
        return OK;
    }

    virtual status_t addListener(const sp<IAdasServiceListener>& listener)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IAdasService::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(listener));
        remote()->transact(BnAdasService::ADD_LISTENER, data, &reply);

        if (IProtocolUtils::readExceptionCode(reply)) return -EPROTO;
        return reply.readInt32();
    }

    virtual status_t removeListener(const sp<IAdasServiceListener>& listener)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IAdasService::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(listener));
        remote()->transact(BnAdasService::REMOVE_LISTENER, data, &reply);

        if (IProtocolUtils::readExceptionCode(reply)) return -EPROTO;
        return reply.readInt32();
    }
 
};

IMPLEMENT_META_INTERFACE(AdasService, "AdasService");

// ----------------------------------------------------------------------

status_t BnAdasService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code)
    {
    case CONNECT:
    {
        CHECK_INTERFACE(IAdasService, data, reply);
        ALOGI("BnAdasService connect  adas");
        sp<IAdasClient> adasClient =
            interface_cast<IAdasClient>(data.readStrongBinder());
        int32_t streamId = data.readInt32();
        const String16 clientName = data.readString16();
        int32_t clientUid = data.readInt32();
        sp<IAdasStream> stream;
        status_t status = connect(adasClient, streamId, clientName, clientUid, stream);
        ALOGI("4");
        if (stream != NULL)
        {
            ALOGI("stream != NULL");
            reply->writeStrongBinder(IInterface::asBinder(stream));
            ALOGI("write stream != NULL ok");
        }
        else
        {
        	//write error if stream is not up
            ALOGI("stream == NULL");
            IProtocolUtils::writeExceptionCode(status, "Unable to initialize stream", reply);
            return BAD_VALUE;
        }
        return NO_ERROR;
    }
    break;

    case ADD_LISTENER:
    {
        CHECK_INTERFACE(IAdasService, data, reply);
        sp<IAdasServiceListener> listener =
            interface_cast<IAdasServiceListener>(data.readStrongBinder());
        reply->writeNoException();
        reply->writeInt32(addListener(listener));
        return NO_ERROR;
    }
    break;
    case REMOVE_LISTENER:
    {
        CHECK_INTERFACE(IAdasService, data, reply);
        sp<IAdasServiceListener> listener =
            interface_cast<IAdasServiceListener>(data.readStrongBinder());
        reply->writeNoException();
        reply->writeInt32(removeListener(listener));
        return NO_ERROR;
    }
    break;

    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

// ----------------------------------------------------------------------------

}; // namespace android
