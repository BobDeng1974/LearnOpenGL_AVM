
#define LOG_TAG "adas"

#include "IProtocolUtils.h"
#include <cutils/log.h>

namespace android {

const char* IProtocolUtils::getErrorMsg(int32_t exceptionCode) {
	const char* errorMsg;
	switch (exceptionCode) {
	case EX_SECURITY:
		errorMsg = "Security";
		break;
	case EX_BAD_PARCELABLE:
		errorMsg = "BadParcelable";
		break;
	case EX_NULL_POINTER:
		errorMsg = "NullPointer";
		break;
	case EX_ILLEGAL_STATE:
		errorMsg = "IllegalState";
		break;
		// Binder should be handling this code inside Parcel::readException
		// but lets have a to-string here anyway just in case.
	case EX_HAS_REPLY_HEADER:
		errorMsg = "HasReplyHeader";
		break;
	default:
		errorMsg = "Unknown";
	}

	return errorMsg;
}

bool IProtocolUtils::readExceptionCode(Parcel& reply) {

	int32_t exceptionCode = reply.readExceptionCode();

	if (exceptionCode != 0) {
		const char* errorMsg = getErrorMsg(exceptionCode);
		ALOGE("Binder transmission reply error %s (%d)", errorMsg,
				exceptionCode);
		return true;
	}

	return false;

}

bool IProtocolUtils::writeExceptionCode(int32_t exception, Parcel* packet) {

	if (exception != 0) {
		const char* errorMsg = getErrorMsg(exception);
		packet->writeInt32(exception);
		packet->writeCString(errorMsg);
		ALOGE("Binder transmission sending error %s (%d)", errorMsg, exception);
		return true;
	} else {
		packet->writeNoException();
		return false;
	}
}

void IProtocolUtils::writeExceptionCode(int32_t code, const char* msg, Parcel* packet){
	packet->writeInt32(code);
	packet->writeCString(msg);
	ALOGE("Binder transmission sending error %s (%d)", msg, code);
}
}

