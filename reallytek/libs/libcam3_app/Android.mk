LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam3_app
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libcamera_metadata libmtk_mmutils libcam.metadata libcam_utils libcam3_utils libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam3_app.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam3_app
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libcamera_metadata libmtk_mmutils libcam.metadata libcam_utils libcam3_utils libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam3_app.so
include $(BUILD_PREBUILT)
