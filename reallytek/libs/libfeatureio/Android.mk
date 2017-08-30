LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libfeatureio
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libcam.utils.sensorlistener libcamera_client libcam_utils libcam.utils libcamdrv libcamdrv_FrmB libcam.halsensor libcamalgo libfeatureiodrv libcam.iopipe libcam.metadata libJpgEncPipe libbinder libnvramagentclient libcam_hwutils lib3a libnvram libcam.exif libcameracustom libcam_mmp libdpframework libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libfeatureio.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libfeatureio
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libcam.utils.sensorlistener libcamera_client libcam_utils libcam.utils libcamdrv libcamdrv_FrmB libcam.halsensor libcamalgo libfeatureiodrv libcam.iopipe libcam.metadata libJpgEncPipe libbinder libnvramagentclient libcam_hwutils lib3a libnvram libcam.exif libcameracustom libcam_mmp libdpframework libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libfeatureio.so
include $(BUILD_PREBUILT)
