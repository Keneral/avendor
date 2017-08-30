LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libacdk
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES_64 = libimageio libcamdrv libm4u libcam.halsensor libbwc libcam.iopipe libcam_utils libcam.metadata libhardware libbinder libcamera_client libmtkcam_fwkutils libcamalgo libcam.exif libfeatureio libfeatureiodrv libcameracustom libcam.camshot libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libacdk.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libacdk
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES = libimageio libcamdrv libm4u libcam.halsensor libbwc libcam.iopipe libcam_utils libcam.metadata libhardware libbinder libcamera_client libmtkcam_fwkutils libcamalgo libcam.exif libfeatureio libfeatureiodrv libcameracustom libcam.camshot libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libacdk.so
include $(BUILD_PREBUILT)
