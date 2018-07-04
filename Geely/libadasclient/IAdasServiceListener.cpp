
#define LOG_TAG "adas"

#include <cutils/log.h> /* log header file*/
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <IAdasService.h>
#include <IAdasServiceListener.h>

namespace android {

class BpAdasServiceListener: public BpInterface<
		IAdasServiceListener> {
public:
	BpAdasServiceListener(const sp<IBinder>& impl) :
			BpInterface<IAdasServiceListener>(impl) {
	}

	virtual void statusChanged(bool videoState) {

		Parcel data, reply;
		data.writeInterfaceToken(
				IAdasServiceListener::getInterfaceDescriptor());

		data.writeInt32(videoState ? 1 : 0);
		ALOGE("inside BpAdasServiceListener statusChanged");
		remote()->transact(STATUS_CHANGED, data, &reply, IBinder::FLAG_ONEWAY);
		reply.readExceptionCode();

	}

};

IMPLEMENT_META_INTERFACE(AdasServiceListener, "AdasServiceListener");

// ----------------------------------------------------------------------

status_t BnAdasServiceListener::onTransact(uint32_t code,
		const Parcel& data, Parcel* reply, uint32_t flags) {
	switch (code) {

	case STATUS_CHANGED: {
		CHECK_INTERFACE(IAdasServiceListener, data, reply);

		int32_t videoState = data.readInt32();
		ALOGE("inside BnAdasServiceListener statusChanged");
		statusChanged(videoState == 0 ? false : true);
		reply->writeNoException();

		return NO_ERROR;
	}
		break;

	default:
		return BBinder::onTransact(code, data, reply, flags);
	}
}

// ----------------------------------------------------------------------------

}
;
// namespace android
