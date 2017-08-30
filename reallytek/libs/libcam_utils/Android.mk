LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam_utils
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libui libsync libhardware libgralloc_extra libcamera_metadata libcamera_client libmtkcam_fwkutils libmtk_mmutils libion libion_mtk libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libcam_utils.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libcam_utils
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libui libsync libhardware libgralloc_extra libcamera_metadata libcamera_client libmtkcam_fwkutils libmtk_mmutils libion libion_mtk libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libcam_utils.so
include $(BUILD_PREBUILT)
