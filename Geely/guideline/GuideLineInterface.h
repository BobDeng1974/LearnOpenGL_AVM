//
// Created by RZhu on 7/1/2016.
//

#ifndef GUIDELINEDEMO_GUIDLINEOVERLAY_H
#define GUIDELINEDEMO_GUIDLINEOVERLAY_H
#include <android/native_window_jni.h>

typedef struct vehicleInfo {
    int         dynamic;
    float        angle;
    float        rearspace;
    float        wheelbase;
    float		 vichelSpeed;
    float		forcastTime;
}vehicleInfo_t;

class GuideLineInterface {

public:
    void guideLineEnable();

    void guideLineDisable();

    void guideLinePause();

    void guideLineWakeUp();

    bool isThreadPause();
    
    bool isThreadAlive();

    GuideLineInterface(ANativeWindow *surface, int width, int height);

    ~GuideLineInterface();

    void setData(vehicleInfo_t& vInfo);
};


#endif //GUIDELINEDEMO_GUIDLINEOVERLAY_H
