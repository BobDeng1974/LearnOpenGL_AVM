#include "IAdasVideoService.h"
#include "IAdasVideoClient.h"
#include "IAdasVideoStream.h"
#include <binder/BinderService.h>

namespace android {

class JNIAdasVideoClient : 
				public BnAdasVideoClient,
				public BinderService<JNIAdasVideoClient>
{
   friend class BinderService<JNIAdasVideoClient>;
public:
	JNIAdasVideoClient();
	virtual ~JNIAdasVideoClient();

	virtual void onReverseGearEvent(bool isEnabled);

    virtual void onVideoDisplayEvent(bool isEnabled);

    virtual void onErrorOccured(int errorCode);
	
	virtual void onSyncPulse( bool status);


protected:
	

private:

};



}