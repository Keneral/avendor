LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = camera.mt6737m
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_RELATIVE_PATH = hw
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES_64 = libcamera_client libmtkcam_fwkutils libcam_utils libcam.utils libcam.metadataprovider libcam.halsensor libcam.hal3a.v3 libbinder libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/camera.mt6737m.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = camera.mt6737m
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_RELATIVE_PATH = hw
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES = libcamera_client libmtkcam_fwkutils libcam_utils libcam.utils libcam.metadataprovider libcam.halsensor libcam.hal3a.v3 libbinder libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/camera.mt6737m.so
include $(BUILD_PREBUILT)
