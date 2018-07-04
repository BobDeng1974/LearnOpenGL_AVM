LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES +=  \
	GlShell.cpp \
	GraphicPDC.cpp \
	GraphicVPAFord.cpp \
	lineBorderOp.cpp \
	lodepng.cpp\
	../common/AdasRunable.cpp				
			

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../include  \
    $(LOCAL_PATH)/../common \
    $(LOCAL_PATH)/../include \
    $(LOCAL_PATH)/../libadasservice \
    $(LOCAL_PATH)/../libadasservice/api1 \
    $(LOCAL_PATH)/../libadasservice/notify/ \
    $(LOCAL_PATH)/../guideline/ \
    $(LOCAL_PATH)/../pdc/ \
    $(LOCAL_PATH)/../vns/ \
    /frameworks/native/include/ \
    /system/core/include/ \
    /frameworks/native/include/  \
    /frameworks/native/opengl/include/ \
    external/jsoncpp/include

LOCAL_CFLAGS += -DPLATFORM_ANDROID

LOCAL_MODULE := libdpdc

# for now, until I do a full rebuild.
LOCAL_PRELINK_MODULE := false

# LOCAL_LDFLAGS += -llog

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils libui libgui libbinder libEGL libcutils libGLESv2

LOCAL_CFLAGS += -Idalvik/libnativehelper/include/nativehelper

include $(BUILD_SHARED_LIBRARY)
