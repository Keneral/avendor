LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam3_hwpipeline
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libcam.metadata libcam.metadataprovider libcam3_utils libcam3_pipeline libcam3_hwnode libcam.halsensor libbwc libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam3_hwpipeline.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam3_hwpipeline
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libcam.metadata libcam.metadataprovider libcam3_utils libcam3_pipeline libcam3_hwnode libcam.halsensor libbwc libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam3_hwpipeline.so
include $(BUILD_PREBUILT)
