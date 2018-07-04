#include "JNIAdasVideoClient.h"
#include <log/log.h>

#define LOG_TAG "adas"

using namespace android;

JNIAdasVideoClient::JNIAdasVideoClient()
{

}

JNIAdasVideoClient::~JNIAdasVideoClient()
{

}

void JNIAdasVideoClient::onReverseGearEvent(bool isEnabled)
{
	ALOGI("JNIAdasVideoClient::onReverseGearEvent");
	if(isEnabled == true){
		ALOGI("JNIAdasVideoClient::onReverseGearEvent");
	}
	
}

void JNIAdasVideoClient::onVideoDisplayEvent(bool isEnabled)
{
	
}

void JNIAdasVideoClient::onErrorOccured(int errorCode)
{
	
}

void JNIAdasVideoClient::onSyncPulse( bool status)
{
	
}