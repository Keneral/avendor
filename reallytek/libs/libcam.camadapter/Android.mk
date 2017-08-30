LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.camadapter
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libbinder libui libmtkjpeg libcamera_client libmtkcam_fwkutils libcam_mmp libcam.utils libcam_utils libcam1_utils libcam.paramsmgr libcam.exif libcam.halsensor libcamdrv libimageio libfeatureio libcam.campipe libcam.camshot libJpgEncPipe libdpframework libcameracustom libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam.camadapter.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.camadapter
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libbinder libui libmtkjpeg libcamera_client libmtkcam_fwkutils libcam_mmp libcam.utils libcam_utils libcam1_utils libcam.paramsmgr libcam.exif libcam.halsensor libcamdrv libimageio libfeatureio libcam.campipe libcam.camshot libJpgEncPipe libdpframework libcameracustom libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam.camadapter.so
include $(BUILD_PREBUILT)
