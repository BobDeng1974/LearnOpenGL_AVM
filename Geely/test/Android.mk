LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_SRC_FILES := main_test.cpp \
				   test_AdasVideoClient.cpp 


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/ \
		            /kernel/include/uapi/linux/

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../libadasservice/ 

LOCAL_CFLAGS += -DPLATFORM_ANDROID

LOCAL_MODULE := adastest

# for now, until I do a full rebuild.
LOCAL_PRELINK_MODULE := false

# LOCAL_LDFLAGS += -llog

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils libui libgui libbinder libdpdc libdguideline
LOCAL_STATIC_LIBRARIES := libadasclient libadasservice

LOCAL_CFLAGS += -Idalvik/libnativehelper/include/nativehelper

include $(BUILD_EXECUTABLE)

