LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libfeatureiodrv
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES_64 = libcamdrv libcam.halsensor libcam.iopipe_FrmB libcam_utils libcamdrv_FrmB libnvramagentclient libnvram libcameracustom libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libfeatureiodrv.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libfeatureiodrv
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES = libcamdrv libcam.halsensor libcam.iopipe_FrmB libcam_utils libcamdrv_FrmB libnvramagentclient libnvram libcameracustom libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libfeatureiodrv.so
include $(BUILD_PREBUILT)
