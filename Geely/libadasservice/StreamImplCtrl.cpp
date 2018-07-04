#define LOG_TAG "adas"

#include "StreamImplCtrl.h"


namespace android {

StreamImplCtrl* StreamImplCtrl::m_pInstance = NULL;

StreamImplCtrl* StreamImplCtrl::Instance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new StreamImplCtrl();
    }
    return m_pInstance;
}

void StreamImplCtrl::deInstance()
{
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

StreamImplCtrl::StreamImplCtrl()
    :m_iRearStatus(0)
    ,m_bStatus(false)
{
    m_pGlViewControl = NULL;
    m_pPDCViewControl = NULL;

    memset(&m_sGLViewInfo, 0 , sizeof(viewInfo));
    memset(&m_sPDCViewInfo, 0 , sizeof(viewInfo));
}

StreamImplCtrl::~StreamImplCtrl()
{
    if(m_pAdasStreamImpl != NULL)
    {
        delete m_pAdasStreamImpl;
        m_pAdasStreamImpl = NULL;
    }

    if(m_pGuidLineOverlay != NULL) {
        delete m_pGuidLineOverlay;
        m_pGuidLineOverlay = NULL;
    }
    if(m_pPDCOverlay != NULL) {
        delete m_pPDCOverlay;
        m_pPDCOverlay = NULL;
    }
}

void StreamImplCtrl::init()
{
    ALOGI("StreamImplCtrl |  init");
    creatStreamImpl();
    m_pOverlayWindow->init();
    initGLView();
    initPDCView();
    enablePDC();
    enableGuideLine();
    //test();
}

void StreamImplCtrl::initGLView()
{
    ALOGI("StreamImplCtrl | initGLView");
    m_sGLViewInfo.width = GUIDELINE_VIEW_WIDTH;
    m_sGLViewInfo.height = GUIDELINE_VIEW_HEIGHT;
    m_sGLViewInfo.name = "AdasGuideLine";
    m_sGLViewInfo.viewPos.x = GUIDELINE_VIEW_POS_X;
    m_sGLViewInfo.viewPos.y = GUIDELINE_VIEW_POS_Y;
    m_sGLViewInfo.format = PIXEL_FORMAT_RGB_888;
    m_sGLViewInfo.layer = 0;

    m_pGlViewControl = m_pOverlayWindow->createViewSurface(m_sGLViewInfo);
    if (m_pGlViewControl == NULL) {
        ALOGI("GL Surface Control create failed");
        return;
    }
    m_pOverlayWindow->registerSurfaceView(m_pGlViewControl);
    SurfaceComposerClient::openGlobalTransaction();
    m_pGlViewControl->setPosition(m_sGLViewInfo.viewPos.x, m_sGLViewInfo.viewPos.y);
    m_pGlViewControl->setLayer(DEFAULT_LAYER - m_sGLViewInfo.layer);
    m_pGlViewControl->setAlpha(0);
    SurfaceComposerClient::closeGlobalTransaction();
    m_pGuidLineOverlay = new GuideLineInterface(sp<ANativeWindow>(m_pGlViewControl->getSurface()).get(), 
                                                                                    m_sGLViewInfo.width, m_sGLViewInfo.height);
}

void StreamImplCtrl::initPDCView()
{
    ALOGI("StreamImplCtrl  | initPDCView");
    m_sPDCViewInfo.width = PDC_VIEW_WIDTH;
    m_sPDCViewInfo.height = PDC_VIEW_HEIGHT;
    m_sPDCViewInfo.name = "AdasPDC";
    m_sPDCViewInfo.viewPos.x = PDC_VIEW_POS_X;
    m_sPDCViewInfo.viewPos.y = PDC_VIEW_POS_Y;
    m_sPDCViewInfo.format = PIXEL_FORMAT_RGB_888;
    m_sPDCViewInfo.layer = 0;

    m_pPDCViewControl = m_pOverlayWindow->createViewSurface(m_sPDCViewInfo);

    if (m_pPDCViewControl == NULL) {
        ALOGI("PDC Surface Control create failed");
        return;
    }
    m_pOverlayWindow->registerSurfaceView(m_pPDCViewControl); 
    SurfaceComposerClient::openGlobalTransaction();
    m_pPDCViewControl->setPosition(m_sPDCViewInfo.viewPos.x, m_sPDCViewInfo.viewPos.y);
    m_pPDCViewControl->setLayer(DEFAULT_LAYER - m_sPDCViewInfo.layer);
    m_pPDCViewControl->setAlpha(0);
    SurfaceComposerClient::closeGlobalTransaction();

    m_pPDCOverlay = new GraphicVPAFord();
    if(m_pPDCOverlay != NULL) {
        m_pPDCOverlay->vInit(sp<ANativeWindow>(m_pPDCViewControl->getSurface()).get(),
                                               m_sPDCViewInfo.width, m_sPDCViewInfo.height);
    }
}

void StreamImplCtrl::creatStreamImpl()
{
    m_pOverlayWindow = new OverlayWindow();
    if(m_pOverlayWindow != NULL) {
        ALOGI("creat  overlaywindow ok");
    }

    m_pAdasStreamImpl = new AdasStreamImpl();
    if (m_pAdasStreamImpl != NULL) {
        ALOGI("creat rvc stream impl ok"); 
    } 
}

AdasStreamImpl* StreamImplCtrl::getAdasStreamImpl(bool isFirst)
{
    if(m_pAdasStreamImpl == NULL) {
        m_pAdasStreamImpl = new AdasStreamImpl();
    }
    return m_pAdasStreamImpl;
}

void StreamImplCtrl::enableGuideLine()
{
    ALOGI("AdasStreamImpl | enableGuideLine");
    if(m_pGuidLineOverlay != NULL) {
        vehicleInfo_t tmp = {2, 0.0f, 1.4f, 1.5f, 0.2f, 10.0f};
        m_pGuidLineOverlay->setData(tmp);
        m_pGuidLineOverlay->guideLineEnable();     
    }
}

void StreamImplCtrl::enablePDC()
{
    ALOGI("StreamImplCtrl | enablePDC");
    if(m_pPDCOverlay != NULL) {
        m_pPDCOverlay->start();
        m_pPDCOverlay->bSetPDCSize(304, 576);
    }
}

void StreamImplCtrl::disablePDC()
{
    ALOGI("StreamImplCtrl | disablePDC");
    if(m_pPDCOverlay != NULL) {
        m_pPDCOverlay->quit();
    }
}

void StreamImplCtrl::disableGuideLine()
{
    ALOGI("StreamImplCtrl | disableGuideLine");
    if(m_pGuidLineOverlay != NULL) {
        m_pGuidLineOverlay->guideLineDisable();
    }
}

void StreamImplCtrl::guidelinePause()
{
    ALOGI("StreamImplCtrl | guidelinePause");
    if(m_pGuidLineOverlay != NULL) {
        m_pGuidLineOverlay->guideLinePause();
    }
}

void StreamImplCtrl::PDCPause()
{
    ALOGI("StreamImplCtrl | PDCPause");
    if(m_pPDCOverlay != NULL) {
        m_pPDCOverlay->pause();
    }
}

void StreamImplCtrl::PDCResume()
{
    ALOGI("StreamImplCtrl | PDCResume");
    if(m_pPDCOverlay != NULL) {
        m_pPDCOverlay->resume();
    }
}

void StreamImplCtrl::guidelineResume()
{
    ALOGI("StreamImplCtrl | guidelineWakeUp");
    if(m_pGuidLineOverlay != NULL) {
        m_pGuidLineOverlay->guideLineWakeUp();
    }
}

void StreamImplCtrl::startView(eViewType type)
{
    // ALOGI("[early] StreamImplCtrl | startView %d", type);
    // if(m_pAdasStreamImpl != NULL){
    //     m_pAdasStreamImpl->showCameraVideo(0);
    // }
}

sp<SurfaceControl> StreamImplCtrl::getGLViewCtrl()
{  
    return m_pGlViewControl;
}

 sp<SurfaceControl> StreamImplCtrl::getPDCViewCtrl()
 {  
    return m_pPDCViewControl;
 }

 void StreamImplCtrl::test()
{
    ALOGI("StreamImplCtrl | test");
    int max_num[9] = {-1,-1,-1,-1,0,0,0,0,0};
    int f_num[9] = {-1,-1,-1,-1,2,2,5,5,0};

    

    // max_num[0] = f_num[0];
    // max_num[1] = f_num[1];
    // max_num[2] = f_num[2];
    // max_num[3] = f_num[3];

    // max_num[4] = rand()%(f_num[4]);
    // max_num[5] = rand()%(f_num[5]);
    // max_num[6] = rand()%(f_num[6]);
    // max_num[7] = rand()%(f_num[7]);
     
    // m_pPDCOverlay->bSetGraphicPDCState(max_num);

   // SurfaceComposerClient::openGlobalTransaction();
   //  m_pPDCViewControl->setAlpha(1);
   //  m_pPDCViewControl->show();
   //  m_pGlViewControl->setAlpha(1);
   //  m_pGlViewControl->show();
   //  SurfaceComposerClient::closeGlobalTransaction();
    
}

void StreamImplCtrl::updateSteerWheelAngle(int angle)
{
    ALOGI("StreamImplCtrl::updateSteerWheelAngle | angle[%d]", angle);
}

void StreamImplCtrl::updateRadar(radarType* radar)
{
    // radarType* radarInfo = (radarType*)malloc(RVC_RADAR_NUM*sizeof(radarType));
    ALOGI("StreamImplCtrl | updateRadar");
    int radarId;
    int max_num[9] = {-1,-1,-1,-1,0,0,0,0,0};
    int f_num[9] = {-1,-1,-1,-1,2,2,5,5,0};
    for(int i =0; i<RVC_RADAR_NUM; i++) {
        radarId = radar[i].radarID;
        max_num[radarId] = (radar[i].distanceLevel)%f_num[radarId];
    }
    m_pPDCOverlay->bSetGraphicPDCState(max_num);

    ALOGI("StreamImplCtrl::updateRadar | radar[%x]", radar);
}


}


