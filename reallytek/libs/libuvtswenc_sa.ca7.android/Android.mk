LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libuvtswenc_sa.ca7.android
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libvcodec_oal libvcodec_utility libstdc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libuvtswenc_sa.ca7.android.so
include $(BUILD_PREBUILT)
