#ifndef  ADAS_NOTIFY_CLIENT_BASE_H
#define ADAS_NOTIFY_CLIENT_BASE_H

#include <utils/RefBase.h>
#include "AdasDefine.h"


namespace android
{
class NotifyClientBase : public RefBase
{
public:	
   /*Notify Rvc Stream  */
    NotifyClientBase();
    virtual ~NotifyClientBase();

public:
    virtual void pushMessage(std::string& parm);

private:
    // check whether the calling process matches mClientPid.
};

}

#endif
