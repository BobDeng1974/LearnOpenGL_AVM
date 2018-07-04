
#ifndef ADAS_IPROTOCOL_UTILS_H
#define ADAS_IPROTOCOL_UTILS_H

#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class IProtocolUtils {

public:
	typedef enum {
		EX_SECURITY = -1,
		EX_BAD_PARCELABLE = -2,
		EX_ILLEGAL_ARGUMENT = -3,
		EX_NULL_POINTER = -4,
		EX_ILLEGAL_STATE = -5,
		EX_HAS_REPLY_HEADER = -128,  // special; see below
	};

	static bool readExceptionCode(Parcel& reply);

	static bool writeExceptionCode(int32_t code, Parcel* packet);

	static void writeExceptionCode(int32_t code, const char* msg,
			Parcel* packet);

private:
	static const char* getErrorMsg(int32_t exceptionCode);
};
}
#endif
