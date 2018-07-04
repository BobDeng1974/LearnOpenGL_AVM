LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_SRC_FILES := JNIAdasVideoClient.cpp \
				   com_harman_adasvideo.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include/

LOCAL_C_INCLUDES += /system/core/include/ \
    				/frameworks/native/include/ 

LOCAL_CFLAGS += -DPLATFORM_ANDROID

LOCAL_MODULE := libadasvideo

# for now, until I do a full rebuild.
LOCAL_PRELINK_MODULE := false

# LOCAL_LDFLAGS += -llog

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils libui libgui libbinder
LOCAL_STATIC_LIBRARIES := libAdasvideoclient

LOCAL_CFLAGS += -Idalvik/libnativehelper/include/nativehelper

include $(BUILD_SHARED_LIBRARY)

