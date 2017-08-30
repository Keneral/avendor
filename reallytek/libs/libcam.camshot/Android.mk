LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.camshot
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libcam.campipe libcamdrv libdpframework libmtkjpeg libm4u libJpgEncPipe libfeatureio libcam_mmp libcam.utils libcam.halsensor libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam.camshot.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.camshot
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libcam.campipe libcamdrv libdpframework libmtkjpeg libm4u libJpgEncPipe libfeatureio libcam_mmp libcam.utils libcam.halsensor libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam.camshot.so
include $(BUILD_PREBUILT)
