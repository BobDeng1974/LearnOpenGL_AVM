#include <log/log.h>
#include <gui/Surface.h>
#include <binder/IServiceManager.h>
#include <utils/RefBase.h>

#include "IAdasVideoService.h"
#include "IAdasVideoClient.h"
#include "IAdasVideoStream.h"
#include "JNIAdasVideoClient.h"
#include "com_harman_adasvideo.h"


#define LOG_TAG "adas"


using namespace android;

static sp<Surface> native_surface;
static sp<Surface> native_videosurface;
static sp<IAdasVideoStream> m_nativeStream;


static void Java_com_harman_adasvideo_connect(JNIEnv *env, jobject obj)
 {
     sp<android::IServiceManager> sm = android::defaultServiceManager();
     sp<android::IBinder> binder;
     sp<IAdasVideoService> svvse;
     sp<IAdasVideoClient> svvct;
     sp<IAdasVideoStream> svvsm;

     do {
        binder = sm->getService(String16("AdasVideoService"));
        if (binder != 0)
            break;
            ALOGI("adas service not published, waiting...");
            usleep(500000); // 0.5 s
      } while(true);

   ALOGI("Adas test is now trying");


   svvse = interface_cast<IAdasVideoService>(binder);
     if (svvse != NULL){
           ALOGI("svvse OK");     
      }
        
      svvct = new JNIAdasVideoClient();
      if (svvse != NULL){
           ALOGI("svvct OK");     
      }
      int res = svvse->connect(svvct, 0, String16("adas"), 1, svvsm);
      ALOGI("svvse->connect  res = %d", res);

      if(svvsm != NULL) {
        m_nativeStream = svvsm;
      }

 }

static void  Java_com_harman_adasvideo_enableGuideLine
  (JNIEnv * env, jobject, jobject jsurface)
  {
     ALOGI("enableguideline gni start ....");
     jclass clazz = env->FindClass("android/view/Surface");
     jfieldID field_surface;

     field_surface = env->GetFieldID(clazz, "mSurface", "I");

   if (field_surface == NULL) {
      ALOGI("field_surface == NULL");
     }

   native_surface = (android::Surface*) env->GetIntField(jsurface, field_surface);

   ALOGI("native_surface = %d", native_surface.get());

   if(native_surface->getIGraphicBufferProducer() != 0 && m_nativeStream != 0){
        ALOGI("native_surface->getIGraphicBufferProducer() = %d", native_surface->getIGraphicBufferProducer().get());
        m_nativeStream->enableGuideLines(native_surface->getIGraphicBufferProducer());
    }
    else{
        ALOGI("m_nativeStream->enableGuideLines failed");
    }
  }

static void  Java_com_harman_adasvideo_setPreviewSurface
  (JNIEnv * env, jobject, jobject jsurface)
  {
     ALOGI("setPreviewSurface gni start ....");
     jclass clazz = env->FindClass("android/view/Surface");
     jfieldID field_surface;

     field_surface = env->GetFieldID(clazz, "mSurface", "I");

   if (field_surface == NULL) {
      ALOGI("field_surface == NULL");
     }

   native_videosurface = (android::Surface*) env->GetIntField(jsurface, field_surface);

   ALOGI("native_surface = %d", native_surface.get());

   if(native_surface->getIGraphicBufferProducer() != 0 && m_nativeStream != 0){
        m_nativeStream->setViewTarget(native_videosurface->getIGraphicBufferProducer());
        ALOGI("m_nativeStream->enableGuideLines OK");
    }
    else{
        ALOGI("m_nativeStream->enableGuideLines failed");
    }
  }

static void Java_com_harman_adasvideo_startPreview(JNIEnv * env, jobject)
  {
    if(m_nativeStream != 0){
        m_nativeStream->startView();
        ALOGI("m_nativeStream->startView OK");
    }
    else{
        ALOGI("m_nativeStream->startView failed");
    }
  }

static void JNICALL Java_com_harman_adasvideo_showGuideLine(JNIEnv * env, jobject)
{
      if(m_nativeStream != 0){
          m_nativeStream->showGuideLine();
          ALOGI("m_nativeStream->showGuideLine OK");
    }
    else{
          ALOGI("m_nativeStream->showGuideLine failed");
    }
}

static JNINativeMethod methods[] = 
{  
  {"enableGuideLine", 
   "(Landroid/view/Surface;)V", 
   (void*)Java_com_harman_adasvideo_enableGuideLine},  
  { "setPreviewSurface",
    "(Landroid/view/Surface;)V",
    (void *)Java_com_harman_adasvideo_setPreviewSurface },
  { "connect",
    "()V",
    (void *)Java_com_harman_adasvideo_connect },
  { "startPreview",
    "()V",
    (void *)Java_com_harman_adasvideo_startPreview },
  { "showGuideLine",
    "()V",
    (void *)Java_com_harman_adasvideo_showGuideLine },
};


static int registerNativeMethods(JNIEnv* env, const char* className,  
    JNINativeMethod* gMethods, int numMethods)  
{  
    jclass clazz;  
  
    clazz = env->FindClass(className);  
    if (clazz == NULL) {  
        LOGI("adasjni registration unable to find class '%s'", className);  
        return JNI_FALSE;  
    }  
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {  
        LOGI("RegisterNatives failed for '%s'", className);  
        return JNI_FALSE;  
    }  
  
    return JNI_TRUE;  
} 

static int registerNatives(JNIEnv* env)  
{  
  if (!registerNativeMethods(env, classPathName,  
                methods, sizeof(methods) / sizeof(methods[0]))) {  
      return JNI_FALSE;  
  }  
  
  return JNI_TRUE;  
}  


