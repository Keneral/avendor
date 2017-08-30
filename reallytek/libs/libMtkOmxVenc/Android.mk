LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libMtkOmxVenc
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libbinder libui libion libion_mtk libgralloc_extra libvcodecdrv libvcodec_utility libhardware libdpframework libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libMtkOmxVenc.so
include $(BUILD_PREBUILT)
