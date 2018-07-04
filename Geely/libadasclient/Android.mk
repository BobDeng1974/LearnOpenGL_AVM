ADASVIDEO_CLIENT_LOCAL_PATH:= $(call my-dir)
include $(call all-subdir-makefiles)
include $(CLEAR_VARS)

LOCAL_PATH := $(ADASVIDEO_CLIENT_LOCAL_PATH)

LOCAL_SRC_FILES:= \
	IAdasService.cpp \
	IAdasStream.cpp \
	IAdasServiceListener.cpp \
	IAdasClient.cpp \
	IProtocolUtils.cpp

LOCAL_MODULE:= libadasclient  

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils libui libgui libbinder

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/ \
    				/system/core/include/ \
    				/frameworks/native/include/  \
    				external/jsoncpp/include

LOCAL_CFLAGS += -Idalvik/libnativehelper/include/nativehelper

LOCAL_CFLAGS += -DPLATFORM_ANDROID

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_PATH := $(ADASVIDEO_CLIENT_LOCAL_PATH)

LOCAL_SRC_FILES:= \
	IAdasService.cpp \
	IAdasStream.cpp \
	IAdasServiceListener.cpp \
	IAdasClient.cpp \
	IProtocolUtils.cpp

LOCAL_MODULE:= libadasclients  

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils libui libgui libbinder

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/ \
    				/system/core/include/ \
    				/frameworks/native/include/  \
    				external/jsoncpp/include

LOCAL_CFLAGS += -Idalvik/libnativehelper/include/nativehelper

LOCAL_CFLAGS += -DPLATFORM_ANDROID

include $(BUILD_SHARED_LIBRARY)


