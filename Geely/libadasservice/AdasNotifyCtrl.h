#ifndef ADAS_NOTIFY_CTRL_H_
#define ADAS_NOTIFY_CTRL_H_

#include <fcntl.h>
#include "AdasDefine.h"

#include "AdasRunable.h"
#include "notify/NotifyClient.h"
#include "NotifyServerBase.h"
#include "IAdasClient.h"


namespace android {
	
class AdasNotifyCtrl : public RefBase
                                   , public AdasRunable
{
public:
    static AdasNotifyCtrl* getInstance();
    static void deInstance();
    virtual ~AdasNotifyCtrl();

    virtual void update();
    virtual void init();

public:
    void distroy();

    void pushMessage(eType type, std::string& parms);

    void registerClient(const sp<IAdasClient>& adasClient);
   
private:
    AdasNotifyCtrl();    
    static AdasNotifyCtrl* m_pInstance;      

     void handleMesg(std::string& sMsg);

    Vector<std::string> nMsgList;
    NotifyServerBase*  m_sNotifyServerBase;

 };

}


#endif