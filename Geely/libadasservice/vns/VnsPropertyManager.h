#ifndef ADAS_VNS_PROPERTY_MANAGER_H_
#define ADAS_VNS_PROPERTY_MANAGER_H_

#include <fcntl.h>
#include "AdasDefine.h"
#include "IVehicleNetwork.h"
#include "VnsPropertyReceiver.h"

namespace android {
	
class VnsPropertyManager : public RefBase
{
public:
    static VnsPropertyManager* getInstance();
    virtual ~VnsPropertyManager();

    void init();
    void subscribe();
    void unSubscribe();
    int getRearProp();

private:
    VnsPropertyManager();    
    static VnsPropertyManager* m_pInstance; 
    sp<IVehicleNetwork> connectToService();

    sp<IVehicleNetwork> m_pVn;
    sp<VnsPropertyReceiver> mListener;

 };

}


#endif