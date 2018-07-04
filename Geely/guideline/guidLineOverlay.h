//
// Created by RZhu on 7/1/2016.
//

#ifndef GUIDELINEDEMO_GUIDLINEOVERLAY_H
#define GUIDELINEDEMO_GUIDLINEOVERLAY_H
#include <android/log.h>
#define TAG  "note"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#include <android/native_window_jni.h>
//#include "RVSDefine.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <utils/Thread.h>
#include "GlGMRVS.h"
#include "LCommon.h"

using namespace android;

class guidLineOverlay : public Thread
{

public:
    EGLDisplay mEglDisplay;
    ANativeWindow * mWin;
    EGLSurface mEglSurface;
    //static bool drawStatus = true;

public:
    virtual bool threadLoop();
    void startSyncThread();
    void stopSyncThread();
    int enableGuideLine(ANativeWindow * surface, int width, int height);
    void disableGuideLine();

    void setView(ANativeWindow * surface);
    void startView();
    void stopView();
    guidLineOverlay();
    ~guidLineOverlay();

private:
    Mutex m_lock;
    int m_width;
    int m_height;
    bool drawStatus;
};


#endif //GUIDELINEDEMO_GUIDLINEOVERLAY_H
