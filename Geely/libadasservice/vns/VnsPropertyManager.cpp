#define LOG_TAG "adas"

#include <binder/IServiceManager.h>
#include <binder/BinderService.h>
#include <binder/ProcessState.h>
#include <utils/threads.h>
#include "VnsPropertyManager.h"
#include "VnsPropertyDefine.h"

namespace android{

int subPropertyValue[] = {
    VEHICLE_PROPERTY_BCM_REVERSEGEARINFO,
    VEHICLE_PROPERTY_IPK_AVERAGEVEHICLESPEED,
    VEHICLE_PROPERTY_SAS_STEERWHEELANGLE ,
    VEHICLE_PROPERTY_SENSOR_ADAS_STATUS_REQUEST,
    VEHICLE_PROPERTY_SENSOR_RADAR_ERROR_STATUS_NOTIFY,
    VEHICLE_PROPERTY_SENSOR_RADAR_SENSOR_DATA_NOTIFY,
    -1
};

VnsPropertyManager* VnsPropertyManager::m_pInstance = NULL;

VnsPropertyManager* VnsPropertyManager::getInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new VnsPropertyManager();
    }
    return m_pInstance;
}

VnsPropertyManager::VnsPropertyManager()
{
    ALOGI("VnsPropertyManager constructor");
}

VnsPropertyManager::~VnsPropertyManager()
{
    unSubscribe();
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
    ALOGI("AdasNotifyCtrl destructor"); 
}

void VnsPropertyManager::init()
{
    ProcessState::self()->startThreadPool();

    m_pVn = connectToService();
    // sp<VehiclePropertiesHolder> properties = m_pVn->listProperties();
    // int32_t numConfigs = properties->getList().size();
    // ALOGI("properties numConfigs[%d]", properties->getList().size());

     mListener = sp<VnsPropertyReceiver>(new VnsPropertyReceiver());
     subscribe();
}

void VnsPropertyManager::subscribe()
{
    ALOGI("VnsPropertyManager | subscribe start");

    for(int i=0; subPropertyValue[i] != -1; i++){
        ALOGI("propertyValue [%d] = [%x] ", i, subPropertyValue[i]);
   
        status_t r = m_pVn->subscribe(mListener, subPropertyValue[i], 0, 0);

        if(r != NO_ERROR) {
            ALOGI("subscribe property[%d]  failed", subPropertyValue[i]);
        }
        usleep(10000);
    }

    return;
}

void VnsPropertyManager::unSubscribe()
{
     ALOGI("VnsPropertyManager | unsubscribe start");

       for(int i=0; subPropertyValue[i] != -1; i++){
            m_pVn->unsubscribe(mListener, subPropertyValue[i]);
            ALOGI("unSubscribe property[%d]  failed", subPropertyValue[i]);
            usleep(10000);
        }

}

int VnsPropertyManager::getRearProp()
{
    ALOGI("VnsPropertyManager | getRearProp start");
    ScopedVehiclePropValue value;
    value.value.prop = VEHICLE_PROPERTY_BCM_REVERSEGEARINFO;
    value.value.value_type = VEHICLE_VALUE_TYPE_BOOLEAN;
    status_t r = m_pVn->getProperty(&value.value);
    if(r != NO_ERROR) {
        return -1;
    }
    return (int)value.value.value.int32_value;
}


 sp<IVehicleNetwork> VnsPropertyManager::connectToService() {
        ALOGI("VnsPropertyManager| connectToService ...........");
        const sp<IServiceManager> sm = defaultServiceManager();

        while (sm->checkService(String16(IVehicleNetwork::SERVICE_NAME)) == NULL) 
        {
            ALOGI("Waiting for IVehicleNetwork...........");
            usleep(10000);      
        }

        sp<IBinder> binder = defaultServiceManager()->getService(
                String16(IVehicleNetwork::SERVICE_NAME));
        if (binder != NULL) {
            ALOGI("binder != NULL");
            sp<IVehicleNetwork> vn(interface_cast<IVehicleNetwork>(binder));
            return vn;
        }else{
            ALOGI("binder == NULL");
        }
        sp<IVehicleNetwork> dummy;
        return dummy;
}



}

