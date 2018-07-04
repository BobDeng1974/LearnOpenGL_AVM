//
// Created by RZhu on 7/1/2016.
//
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "guidLineOverlay.h"
#define VERTEX_ARRAY	0

using namespace android;

int guidLineOverlay::enableGuideLine(ANativeWindow * surface, int width, int height)
{
    /****************************************************************/
    //Step 0 - Create a EGLNativeWindowType that we can use for OpenGL ES output
    if(!surface){
        LOGV("surface translate is NULL");
        return -1;
    }else{
        LOGV("surface translate is OK");
    }

    mWin = surface;
    /****************************************************************/
    // Step 1 - Get the default display.
    m_height = height;
    m_width = width;

    return 0;

}
void guidLineOverlay::disableGuideLine()
{
    //eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   // eglTerminate(eglDisplay);

}

void guidLineOverlay::setView(ANativeWindow * surface)
{

}

void guidLineOverlay::startView()
{

}
void guidLineOverlay::stopView()
{

}
guidLineOverlay::guidLineOverlay()
{

}
guidLineOverlay::~guidLineOverlay()
{

}

void guidLineOverlay::startSyncThread() {
    AutoMutex _l(m_lock);
    drawStatus = true;
    run("GuideLine",PRIORITY_DEFAULT);
    LOGV("DrawGuideLine | startSyncThread");
}
void guidLineOverlay::stopSyncThread() {
    AutoMutex _l(m_lock);
    drawStatus = false;
}

bool guidLineOverlay::threadLoop() {

    bool flag = true;

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
        LOGV("Create a surface to draw to.");
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

    /******************************************************************************/
    //Step 9 - Draw something with OpenGL ES.
    // Matrix used for projection model view (PMVMatrix)
  
    GlGMRVS::getInstance()->Init(m_width, m_height, GUIDELINE_PARA);

    while (1){
        usleep(40000);

        if(flag){
            GUIDELINE_PARA.angle+=0.5;
            if(GUIDELINE_PARA.angle>35){
                flag = false;
            }
        } else
        {
            GUIDELINE_PARA.angle-=0.5;
            if(GUIDELINE_PARA.angle<-35){
                flag = true;
            }
        }

        GlGMRVS::getInstance()->UpdatePositionData(m_width,m_height,GUIDELINE_PARA);
        GlGMRVS::getInstance()->Update();


        eglSwapBuffers(mEglDisplay, mEglSurface);
    }

    return false;
}