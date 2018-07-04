//
// Created by RZhu on 7/29/2016.
//

#ifndef GUIDELINEDEMO_DRAWGUIDELINE_H
#define GUIDELINEDEMO_DRAWGUIDELINE_H
#include <fcntl.h>
#include <utils/Thread.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "GlGMRVS.h"
#include "LCommon.h"
#include <android/log.h>
#define TAG  "adas"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

using namespace android;

class DrawGuideLine : public  Thread{

public:
    static DrawGuideLine* getInstance();
    void deInstance();
	int init(ANativeWindow * surface, int width, int height);
	int eglInit();
	void startSyncThread();
    void stopSyncThread();
    void setData(dynamic_para_t& vInfo);
    void setLastData(dynamic_para_t& vInfo);
    dynamic_para_t getData();
    void pause();
    void wakeUp();
    bool isThreadPause();
    bool isThreadAlive();
    ~DrawGuideLine();

private:
    dynamic_para_t vehiclePara;
    static DrawGuideLine *instance;

private:
    DrawGuideLine();
    void distroy();
    virtual bool threadLoop();

private:
    bool m_bQuit;
    bool pauseStatus;
    bool m_bAlive;
    Condition m_signal;
    Mutex m_lock;
    EGLDisplay mEglDisplay;
    ANativeWindow * mWin;
    EGLSurface mEglSurface;
    dynamic_para_t lastVehiclePara;
    int m_width;
    int m_height;
};

#endif //GUIDELINEDEMO_DRAWGUIDELINE_H