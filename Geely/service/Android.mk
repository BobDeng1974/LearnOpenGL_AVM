LOCAL_PATH := $(call my-dir)

#
# rvehvideosvc binary executable
#
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
		adasmain.cpp \


LOCAL_SHARED_LIBRARIES := libutils  libbinder  liblog libcutils libdguideline libdpdc libgui libvehiclenetwork-native \
			           libtiipcutils libtiipc libtitransportrpmsg


LOCAL_STATIC_LIBRARIES := libadasservice  libadasclient libjsoncpp  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../libadasservice/ \
			external/jsoncpp/include \
			hardware/ti/ipc/linux/include \
			hardware/ti/ipc/packages

LOCAL_MODULE:= adasmain

LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)
##########################################################s
