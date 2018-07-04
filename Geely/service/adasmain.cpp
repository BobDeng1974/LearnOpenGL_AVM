#define LOG_TAG "adas"

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "AdasServiceImpl.h"
#include "log/log.h"
#include <utils/SystemClock.h>

using namespace android;

int main(int /*argc*/, char ** /*argv[]*/) {
	AdasServiceImpl::instantiate();
	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();

	return 0;
}
