#define LOG_TAG "adas"

#include "NotifyServer.h"

namespace android {

struct              
{  
        void (NotifyServer::*fun)(Json::Value& jsonParams);  
        char *tip;        
}CmdTip[] =  {  
                        {&NotifyServer::NotifyRearGearEvent, "NotifyRearGearEvent" },
                        {&NotifyServer::NotifyRVCExitEvent, "NotifyRVCExitEvent" },
                        {&NotifyServer::NotifyVehicleSpeedEvent, "NotifyVehicleSpeedEvent" },
                        {&NotifyServer::NotifyCameraErrorEvent, "NotifyCameraErrorEvent" },
                        {&NotifyServer::NotifyRadarErrorEvent, "NotifyRadarErrorEvent"},
                        {0, 0}
                     };

NotifyServer::NotifyServer()
{
    ALOGI("NotifyServer:NotifyServer ok");
}

NotifyServer::~NotifyServer() 
{

}

int NotifyServer::hanleMsg(const std::string &msg)
{

   Json::Value jsonParams;
   Json::Reader reader;

   if (!reader.parse(msg, jsonParams, false))
   {
      jsonParams.clear();
      ALOGI("NotifyServer::hanleMsg: failed to format json value\n");
      return -1;
   }
   std::string msgtmp =  jsonParams["EventTypeName"].asString();

   for(int i=0 ; CmdTip[i].fun!=NULL; i++){
        if(strcmp(msgtmp.c_str(), CmdTip[i].tip) == 0) {
            (this->*(CmdTip[i].fun))(jsonParams);
        }else{
        }
   }

   return 0;
}

void NotifyServer::NotifyRearGearEvent(Json::Value& jsonParams)
{
    int RearGear =  jsonParams["isEnabled"].asInt();
    ALOGI("NotifyServer | NotifyRearGearEvent RearGear[%d]", RearGear);

    if(!m_vClientList.empty()) {
            Vector<sp<IAdasClient> >::iterator it;
            for (it = m_vClientList.begin(); it != m_vClientList.end(); ++it) {      
                    (*it)->onUpdateGearMark(RearGear);    
            }
    }
    else{
        ALOGI("[notify failed] NotifyServer | m_vClientList is empty");
    }
}

void NotifyServer::NotifyRVCExitEvent(Json::Value& jsonParams)
{
    int reason =  jsonParams["reason"].asInt();
    ALOGI("NotifyServer | NotifyRVCExitEvent reason[%d]", reason);

    if(!m_vClientList.empty()) {
            Vector<sp<IAdasClient> >::iterator it;
            for (it = m_vClientList.begin(); it != m_vClientList.end(); ++it) { 

                    (*it)->onExitAllRVCView(reason);    
            }
    }
    else{
        ALOGI("[notify failed] NotifyServer | m_vClientList is empty");
    }
}

void NotifyServer::NotifyAVMExitEvent(Json::Value& jsonParams)
{
    int reason =  jsonParams["reason"].asInt();
    ALOGI("NotifyServer | NotifyAVMExitEvent reason[%d]", reason);

    if(!m_vClientList.empty()) {
            Vector<sp<IAdasClient> >::iterator it;
            for (it = m_vClientList.begin(); it != m_vClientList.end(); ++it) { 

                    (*it)->onExitAllAVMView(reason);    
            }
    }
    else{
       ALOGI("[notify failed] NotifyServer | m_vClientList is empty");
    }
}

void NotifyServer::NotifyVehicleSpeedEvent(Json::Value& jsonParams)
{
    double speed =  jsonParams["speed"].asDouble();
    ALOGI("NotifyServer | NotifyVehicleSpeedEvent speed[%8f]", speed);

    if(!m_vClientList.empty()) {
            Vector<sp<IAdasClient> >::iterator it;
            for (it = m_vClientList.begin(); it != m_vClientList.end(); ++it) { 

                    (*it)->onUpdateVehicleSpeed(speed);    
            }
    }
    else{
        ALOGI("[notify failed] NotifyServer | m_vClientList is empty");
    }
}

void NotifyServer::NotifyCameraErrorEvent(Json::Value& jsonParams)
{
    int id =  jsonParams["cameraID"].asInt();
    int cameraState =  jsonParams["cameraState"].asInt();
    ALOGI("NotifyServer | NotifyCameraErrorEvent id[%d]  cameraState[%d]", id, cameraState);

    if(!m_vClientList.empty()) {
            Vector<sp<IAdasClient> >::iterator it;
            for (it = m_vClientList.begin(); it != m_vClientList.end(); ++it) { 

                    (*it)->onCameraFaultAlarm_(id, cameraState);    
            }
    }
    else{
        ALOGI("[notify failed] NotifyServer | m_vClientList is empty");
    }
}

void NotifyServer::NotifyRadarErrorEvent(Json::Value& jsonParams)
{
    int id =  jsonParams["radarID"].asInt();
    int radarState =  jsonParams["radarState"].asInt();
    ALOGI("NotifyServer | NotifyRadarErrorEvent id[%d]  radarState[%d]", id, radarState);

    if(!m_vClientList.empty()) {
            Vector<sp<IAdasClient> >::iterator it;
            for (it = m_vClientList.begin(); it != m_vClientList.end(); ++it) { 

                    (*it)->onRadarFaultAlarm_(id, radarState);    
            }
    }
    else{
        ALOGI("[notify failed] NotifyServer | m_vClientList is empty");
    }
}



}