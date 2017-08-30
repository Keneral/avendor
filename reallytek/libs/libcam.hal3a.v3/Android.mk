LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.hal3a.v3
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES_64 = libfeatureiodrv libcamdrv libcam_utils libcam.utils libcamdrv_FrmB libcam.halsensor libcamalgo libcam.metadata libcam.metadataprovider libcam.utils.sensorlistener libcam.iopipe_FrmB libcam.hal3a.v3.dng lib3a libnvram libcam.exif libcameracustom libcam_mmp libdpframework libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam.hal3a.v3.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.hal3a.v3
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES = libfeatureiodrv libcamdrv libcam_utils libcam.utils libcamdrv_FrmB libcam.halsensor libcamalgo libcam.metadata libcam.metadataprovider libcam.utils.sensorlistener libcam.iopipe_FrmB libcam.hal3a.v3.dng lib3a libnvram libcam.exif libcameracustom libcam_mmp libdpframework libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam.hal3a.v3.so
include $(BUILD_PREBUILT)
