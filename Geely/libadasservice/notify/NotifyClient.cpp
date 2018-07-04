
#define LOG_TAG "adas"

#include <sys/sysinfo.h>


#include "NotifyClient.h"

namespace android {

NotifyClient::NotifyClient()
{
    ALOGI("NotifyClient::NotifyClient ok");
}

NotifyClient::~NotifyClient() 
{

}

status_t NotifyClient::NotifyReverseGearEvent(int isEnabled)
{
    Json::Value jsonParams;
    Json::FastWriter fastWriter;
    jsonParams["EventTypeName"] = std::string("NotifyRearGearEvent");

    jsonParams["isEnabled"] = isEnabled;

    std::string ret = fastWriter.write(jsonParams);
    pushMessage(ret);

    return OK;
}

status_t NotifyClient::NotifyRVCExitEvent(int reason)
{
    Json::Value jsonParams;
    Json::FastWriter fastWriter;
    jsonParams["EventTypeName"] = std::string("NotifyRVCExitEvent");

    jsonParams["reason"] = reason;

    std::string ret = fastWriter.write(jsonParams);
    pushMessage(ret);

    return OK;
}

status_t NotifyClient::NotifyAVMExitEvent(int reason)
{
    Json::Value jsonParams;
    Json::FastWriter fastWriter;
    jsonParams["EventTypeName"] = std::string("NotifyAVMExitEvent");

    jsonParams["reason"] = reason;

    std::string ret = fastWriter.write(jsonParams);
    pushMessage(ret);

    return OK;
}

status_t NotifyClient::NotifyVehicleSpeedEvent(double speed)
{
    ALOGI("1");
    Json::Value jsonParams;
    Json::FastWriter fastWriter;
    jsonParams["EventTypeName"] = std::string("NotifyVehicleSpeedEvent");

    jsonParams["speed"] = speed;

    std::string ret = fastWriter.write(jsonParams);
    ALOGI("2");
    pushMessage(ret);
    ALOGI("3");
    return OK;
}

status_t NotifyClient::NotifyCameraErrorEvent(int cameraID, int cameraState)
{
    Json::Value jsonParams;
    Json::FastWriter fastWriter;
    jsonParams["EventTypeName"] = std::string("NotifyCameraErrorEvent");

    jsonParams["cameraID"] = cameraID;
    jsonParams["cameraState"] = cameraState;

    std::string ret = fastWriter.write(jsonParams);
    pushMessage(ret);

    return OK;
}

status_t NotifyClient::NotifyRadarErrorEvent(int radarID, int radarState)
{
    Json::Value jsonParams;
    Json::FastWriter fastWriter;
    jsonParams["EventTypeName"] = std::string("NotifyRadarErrorEvent");

    jsonParams["radarID"] = radarID;
    jsonParams["radarState"] = radarState;

    std::string ret = fastWriter.write(jsonParams);
    pushMessage(ret);

    return OK;
}




};
// namespace android
