LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libMtkOmxVdecEx
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libui libgui_ext libion libion_mtk libdpframework libgralloc_extra libstagefright libvcodecdrv libvcodec_utility libhardware libmhalImageCodec libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libMtkOmxVdecEx.so
include $(BUILD_PREBUILT)
