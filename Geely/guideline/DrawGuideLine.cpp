//
// Created by RZhu on 7/29/2016.
//
#include "DrawGuideLine.h"

using namespace android;

DrawGuideLine* DrawGuideLine::instance = NULL;

DrawGuideLine* DrawGuideLine::getInstance()
{
    if(!instance)
    {
        instance = new DrawGuideLine;
    }
   
    return instance;
}

DrawGuideLine::DrawGuideLine(){
    mWin = NULL;
    m_width = 0;
    m_height = 0;
    m_bAlive = false;
    pauseStatus = false;
    m_bQuit = false;
    memset(&lastVehiclePara, -1,sizeof(lastVehiclePara));
}

DrawGuideLine::~DrawGuideLine() {

}

void DrawGuideLine::deInstance() {   
    LOGV("deInstance");
    if(instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

int DrawGuideLine::init(ANativeWindow *surface, int width, int height){
	AutoMutex _l(m_lock);
     if(!surface){
        LOGV("surface translate is NULL");
        return -1;
    }else{
        mWin = surface;
        m_width = width;
        m_height = height;
        LOGV("surface translate is OK");
    }
    return 0;
}

int DrawGuideLine::eglInit() {
   
    /****************************************************************/
    // Step 1 - Get the default display.

    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(EGL_NO_DISPLAY == mEglDisplay) {
        LOGV("The default display is wrong!");
        return -1;
    } else{
        LOGV("Get the default display!");
    }

    /****************************************************************/
    //Step 2 - Initialize EGL.
    GLint majorVersion;
    GLint minorVersion;

    if(!eglInitialize(mEglDisplay, &majorVersion, &minorVersion)) {
        LOGV("Initialize EGL is ERROR");
        return -1;
    } else {
        LOGV("Initialize EGL is OK");
    }

    /**************************************************************/
    //Step 3 - Make OpenGL ES the current API.

    /*******************************************************/
    //Step 4 - Specify the required configuration attributes.
    EGLint config_attribs[] = {
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    /***********************************************************/
    //Step 5 - Find a config that matches all requirements.
    int num_configs = 0;
    EGLConfig   eglConfig;
    if(!eglChooseConfig(mEglDisplay, config_attribs, &eglConfig, 1, &num_configs))
    {
        LOGV("Can't find a config that matches all requirements");
        return -1;
    } else{
        LOGV("Find a config that matches all requirements");
    }

    /*************************************************************/
    //Step 6 - Create a surface to draw to.
    mEglSurface = eglCreateWindowSurface(mEglDisplay, eglConfig, mWin, NULL);
    if(EGL_NO_SURFACE == mEglSurface) {
        LOGV("Can't Create a surface");
        return -1;
    } else{
        LOGV("Create a window surface to draw to.");
    }

    /* if(!eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &mSurfaceWidth) ||
        !eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &mSurfaceHeight)) {
         return -1;
     }*/

    /********************************************************************/
    //Step 7 -Create a context.
    EGLint context_attrib[] = {EGL_CONTEXT_CLIENT_VERSION, 2,EGL_NONE  };
    EGLContext mEglContext;
    mEglContext = eglCreateContext(mEglDisplay, eglConfig, EGL_NO_CONTEXT, context_attrib);
    if(EGL_NO_CONTEXT == mEglContext) {
        LOGV("Can't create context!");
        return -1;
    } else{
        LOGV("Create Context!");
    }

    /*************************************************************************/
    //Step 8 - Bind the context to the current thread and use our  window surface for drawing and reading.
    if(!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        return -1;
    } else {
        LOGV(" Bind the context!");
    }
    return 0;
}

void DrawGuideLine::startSyncThread() {
    LOGV("start Guideline Thread......");
    AutoMutex _l(m_lock);
    m_bQuit = false;
    run("GuideLine",PRIORITY_DEFAULT);
}

void DrawGuideLine::stopSyncThread() {
    LOGV("stopSyncThread......");
    AutoMutex _l(m_lock);
    // requestExit();
    m_bQuit = true;
    requestExit();
    LOGV("stopSyncThread end");
}

void DrawGuideLine::pause() {
    LOGV("guideline thread pause.....");

    // requestExit();
    pauseStatus = true;

    LOGV("guideline thread pause....end");
}

void DrawGuideLine::wakeUp() {
    LOGV("wakeUp....");
    m_signal.signal();
    pauseStatus = false;
}

bool DrawGuideLine::isThreadPause() {
    return pauseStatus;
}

bool DrawGuideLine::isThreadAlive() {
    return m_bAlive;
}

dynamic_para_t DrawGuideLine::getData() {
    AutoMutex _l(m_lock);
    return vehiclePara;
}

void DrawGuideLine::setData(dynamic_para_t& vInfo) {
    AutoMutex _l(m_lock);
    vehiclePara.dynamic = vInfo.dynamic;
    vehiclePara.angle = vInfo.angle;
    vehiclePara.forcastTime = vInfo.forcastTime;
    vehiclePara.rearspace = vInfo.rearspace;
    vehiclePara.vichelSpeed = vInfo.vichelSpeed;
    vehiclePara.wheelbase = vInfo.wheelbase;
}

void DrawGuideLine::setLastData(dynamic_para_t& vInfo) {
    lastVehiclePara.dynamic = vInfo.dynamic;
    lastVehiclePara.angle = vInfo.angle;
    lastVehiclePara.forcastTime = vInfo.forcastTime;
    lastVehiclePara.rearspace = vInfo.rearspace;
    lastVehiclePara.vichelSpeed = vInfo.vichelSpeed;
    lastVehiclePara.wheelbase = vInfo.wheelbase;
}

bool DrawGuideLine::threadLoop() {
    eglInit();
    bool flag = false;
    dynamic_para_t vehiclePara;
    vehiclePara = getData();
    GlGMRVS::getInstance()->Init(m_width, m_height, vehiclePara);
    while (!m_bQuit){
        if(pauseStatus) {
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                eglSwapBuffers(mEglDisplay, mEglSurface); 
                m_signal.wait(m_lock);
        }

        // vehiclePara = getData();
         usleep(40000);
        if(flag){
            vehiclePara.angle+=0.5f;
            if(vehiclePara.angle>35.0f){
                flag = false;
            }
        } else
        {
            vehiclePara.angle-=0.5f;
            if(vehiclePara.angle<-35.0f){
                flag = true;
            }
        }
  
        if(lastVehiclePara.angle == vehiclePara.angle){
            usleep(15000);
            continue;
        } else{
            setLastData(vehiclePara);
            GlGMRVS::getInstance()->UpdatePositionData(m_width, m_height, vehiclePara);
            GlGMRVS::getInstance()->Update();
            eglSwapBuffers(mEglDisplay, mEglSurface);
        }
 
    }

    distroy();
    m_bAlive = false;
    LOGV("guideline thread exit.....");
    return false;
}

void DrawGuideLine::distroy()
{
    LOGV("stop threadLoop before return -1 ");
    glClear(GL_COLOR_BUFFER_BIT);
    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    LOGV("stop threadLoop before return 0 ");
    eglTerminate(mEglDisplay);
    LOGV("stop threadLoop before return 1 ");
}