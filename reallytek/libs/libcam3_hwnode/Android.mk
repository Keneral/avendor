LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam3_hwnode
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES_64 = libcam_utils libcam.metadata libcam.metadataprovider libcam.hal3a.v3 libcam.iopipe_FrmB libcam.halsensor libfeatureio libcamdrv libcam.iopipe libcam.exif.v3 libmtkjpeg libcamdrv_FrmB libcam.utils.sensorlistener libhdrproc libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam3_hwnode.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam3_hwnode
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES = libcam_utils libcam.metadata libcam.metadataprovider libcam.hal3a.v3 libcam.iopipe_FrmB libcam.halsensor libfeatureio libcamdrv libcam.iopipe libcam.exif.v3 libmtkjpeg libcamdrv_FrmB libcam.utils.sensorlistener libhdrproc libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam3_hwnode.so
include $(BUILD_PREBUILT)
