LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.client
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libbinder libui libion libion_mtk libhardware libcamera_client libcam_mmp libcam.utils libcam_utils libcam.paramsmgr libcamdrv libfeatureio libdpframework libm4u libJpgEncPipe libcam.exif libcameracustom libmtkcam_fwkutils libgralloc_extra libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam.client.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.client
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libbinder libui libion libion_mtk libhardware libcamera_client libcam_mmp libcam.utils libcam_utils libcam.paramsmgr libcamdrv libfeatureio libdpframework libm4u libJpgEncPipe libcam.exif libcameracustom libmtkcam_fwkutils libgralloc_extra libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam.client.so
include $(BUILD_PREBUILT)
