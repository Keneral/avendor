LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libdpframework
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES_64 = libion libm4u libsync libion_mtk libbinder libvcodecdrv libc++
LOCAL_EXPORT_C_INCLUDE_DIRS = $(LOCAL_PATH)/include
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libdpframework.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libdpframework
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SHARED_LIBRARIES = libion libm4u libsync libion_mtk libbinder libvcodecdrv libc++
LOCAL_EXPORT_C_INCLUDE_DIRS = $(LOCAL_PATH)/include
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libdpframework.so
include $(BUILD_PREBUILT)
