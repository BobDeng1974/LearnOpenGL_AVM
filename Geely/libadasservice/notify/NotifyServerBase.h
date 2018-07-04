#ifndef ADAS_NOTIFY_SERVER_BASE_H
#define ADAS_NOTIFY_SERVER_BASE_H

#include <utils/RefBase.h>
#include "AdasDefine.h"
#include "IAdasClient.h"

namespace android
{
class NotifyServerBase : public RefBase
{
public:	
   /*Notify Rvc Stream  */
    NotifyServerBase();
    virtual ~NotifyServerBase();

    virtual int hanleMsg(const std::string &strData) = 0;

public:
    int onSignal(const std::string &strData);
    void registerClient(const sp<IAdasClient>& adasClient);

protected:
     Vector<sp<IAdasClient> > m_vClientList;
    // check whether the calling process matches mClientPid.
};

}

#endif
