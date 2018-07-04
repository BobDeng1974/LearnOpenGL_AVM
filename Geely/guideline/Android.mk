LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := DrawGuideLine.cpp \
				   GuideLineInterface.cpp

LOCAL_SRC_FILES +=  GlGMRVS.cpp\
					GlGuideLine.cpp\
					GlLineGroup.cpp\
					GlShell.cpp\
					LCommon.cpp\
					line2D.cpp\
					lineBorderOp.cpp\
					PVRTError.cpp\
					PVRTFixedPoint.cpp\
					PVRTMatrixF.cpp\
					PVRTMatrixX.cpp\
					PVRTString.cpp\
					PVRTVector.cpp\
					trajectory.cpp	
					
					


LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include  \
    /frameworks/native/include/ \
    /system/core/include/ \
    /frameworks/native/include/  \
    /frameworks/native/opengl/include/ \
    /vendor/harman/adas/source/native/ \
    

LOCAL_CFLAGS += -DPLATFORM_ANDROID

LOCAL_MODULE := libdguideline

# for now, until I do a full rebuild.
LOCAL_PRELINK_MODULE := false

# LOCAL_LDFLAGS += -llog

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils libui libgui libbinder libEGL libcutils libGLESv2

LOCAL_CFLAGS += -Idalvik/libnativehelper/include/nativehelper

include $(BUILD_SHARED_LIBRARY)
