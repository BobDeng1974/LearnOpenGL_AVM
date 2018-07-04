LOCAL_PATH:= $(call my-dir)


include $(CLEAR_VARS)


LOCAL_SRC_FILES:= \
	./api1/AdasStreamImpl.cpp \
	./api1/OverlayWindow.cpp \
	./notify/NotifyClient.cpp \
	./notify/NotifyClientBase.cpp \
	./notify/NotifyServerBase.cpp \
	./notify/NotifyServer.cpp \
	AdasNotifyCtrl.cpp \
	AdasServiceImpl.cpp \
	StreamImplCtrl.cpp \
	./vns/VnsPropertyManager.cpp \
	./vns/VnsPropertyReceiver.cpp \
	./vns/VnsPropertySender.cpp \
	./a15_m4_ipc/A15M4Ipc.cpp

LOCAL_SHARED_LIBRARIES:= \
    libui \
    liblog \
    libutils \
    libbinder \
    libcutils \
    libgui \
    libhardware \
    libdguideline \
    libdpdc \
    libcamera_metadata \
    libvehicle_client \
    libvehiclenetwork-native \
    libtiipcutils libtiipc libtitransportrpmsg



LOCAL_STATIC_LIBRARIES := libadasclient libjsoncpp
   
LOCAL_C_INCLUDES := \
    	$(LOCAL_PATH)/ \
    	$(LOCAL_PATH)/../common/ \
    	$(LOCAL_PATH)/api1/ \
    	$(LOCAL_PATH)/notify/ \
	$(LOCAL_PATH)/../include \
	$(LOCAL_PATH)/../guideline/ \
	$(LOCAL_PATH)/../pdc/ \
	$(LOCAL_PATH)/../vns/ \
	$(LOCAL_PATH)/../a15_m4_ipc/ \
	frameworks/native/services/surfaceflinger/ \
	device/harman/poc/vehicle_server/include \
	external/jsoncpp/include \
	packages/services/Car/libvehiclenetwork/include \
	hardware/harman/vehicle/ \
	hardware/ti/ipc/linux/include \
	hardware/ti/ipc/packages


LOCAL_MODULE:= libadasservice

LOCAL_CFLAGS:= -Idalvik/libnativehelper/include/nativehelper

LOCAL_CFLAGS += -DPLATFORM_ANDROID

include $(BUILD_STATIC_LIBRARY)



