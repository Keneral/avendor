LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.device3
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libcam_utils libcam.metadataprovider libcam.metadata libcamera_metadata libcam3_app libcam3_utils libcam3_pipeline libcam3_hwpipeline libcam.halsensor libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam.device3.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam.device3
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libcam_utils libcam.metadataprovider libcam.metadata libcamera_metadata libcam3_app libcam3_utils libcam3_pipeline libcam3_hwpipeline libcam.halsensor libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam.device3.so
include $(BUILD_PREBUILT)
