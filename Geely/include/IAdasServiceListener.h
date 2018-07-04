
#ifndef ADAS_ADASSERVICE_LISTENER_H
#define ADAS_ADASSERVICE_LISTENER_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class IAdasServiceListener: public IInterface {

public:

	enum {
		STATUS_CHANGED = IBinder::FIRST_CALL_TRANSACTION,
	};

	DECLARE_META_INTERFACE(AdasServiceListener);

	virtual void statusChanged(bool videoState) = 0;
};

// ----------------------------------------------------------------------------

class BnAdasServiceListener: public BnInterface<IAdasServiceListener> 
{
public:
	virtual status_t onTransact(uint32_t code, const Parcel& data,
			Parcel* reply, uint32_t flags = 0);
};

};
// namespace android

#endif
