//
// Created by RZhu on 7/1/2016.
//

#include "GuideLineInterface.h"
#include "DrawGuideLine.h"
#include "LCommon.h"

void GuideLineInterface::setData(vehicleInfo_t& vInfo) {
    dynamic_para_t vehiclePara;
    vehiclePara.dynamic = vInfo.dynamic;
    vehiclePara.angle = vInfo.angle;
    vehiclePara.forcastTime = vInfo.forcastTime;
    vehiclePara.rearspace = vInfo.rearspace;
    vehiclePara.vichelSpeed = vInfo.vichelSpeed;
    vehiclePara.wheelbase = vInfo.wheelbase;
    DrawGuideLine::getInstance()->setData(vehiclePara);
}

void GuideLineInterface::guideLineEnable( ) {
    DrawGuideLine::getInstance()->startSyncThread();
}

void GuideLineInterface::guideLineDisable( ) {
    DrawGuideLine::getInstance()->stopSyncThread();
    DrawGuideLine::getInstance()->deInstance();
}

GuideLineInterface::GuideLineInterface(ANativeWindow * surface, int width, int height)
{
    DrawGuideLine::getInstance()->init(surface, width, height);
}

void GuideLineInterface::guideLinePause()
{
    DrawGuideLine::getInstance()->pause();
}

void GuideLineInterface::guideLineWakeUp()
{   
    DrawGuideLine::getInstance()->wakeUp();
}

bool GuideLineInterface::isThreadPause()
{
    return DrawGuideLine::getInstance()->isThreadPause();
}

bool GuideLineInterface::isThreadAlive()
{
    return DrawGuideLine::getInstance()->isThreadAlive();
}

GuideLineInterface::~GuideLineInterface()
{

}
