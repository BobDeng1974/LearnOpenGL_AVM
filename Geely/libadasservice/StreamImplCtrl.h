#ifndef ADAS_STREAM_IMPL_FACTORY_H_
#define ADAS_STREAM_IMPL_FACTORY_H_

#include "AdasStreamImpl.h"
#include "OverlayWindow.h"
#include "AdasDefine.h"
#include "GuideLineInterface.h"
#include "GraphicVPAFord.h"

namespace android{

class StreamImplCtrl : public RefBase
{
public:
    static StreamImplCtrl* Instance();
    static void deInstance();
    virtual ~StreamImplCtrl();

    void init();
    void startView(eViewType type);
    void stopView(eViewType type);
    AdasStreamImpl*  getAdasStreamImpl(bool isFirst);
    void setReady(bool status){m_bStatus = status;};
    bool isReady(){return m_bStatus ? true : false;};

    void enableGuideLine();
    void disableGuideLine();
    void guidelinePause();
    void guidelineResume();

    void enablePDC();
    void disablePDC();
    void PDCPause();
    void PDCResume();

    sp<SurfaceControl> getGLViewCtrl();
    sp<SurfaceControl> getPDCViewCtrl();

    void test();//for pdc test

    void updateSteerWheelAngle(int angle);
    void updateRadar(radarType* radar);

private:
    void initGLView();
    void initPDCView();



private:
    StreamImplCtrl();
    void creatStreamImpl();
    static StreamImplCtrl* m_pInstance;
    AdasStreamImpl* m_pAdasStreamImpl;
    sp<OverlayWindow> m_pOverlayWindow;

    viewInfo                                 m_sGLViewInfo;
    viewInfo                                 m_sPDCViewInfo;
    sp<SurfaceControl>              m_pGlViewControl;
    sp<SurfaceControl>              m_pPDCViewControl;
    GuideLineInterface*             m_pGuidLineOverlay;
    GraphicVPAFord*                  m_pPDCOverlay;

    bool m_bStatus;

    int m_iRearStatus;
};





}

#endif