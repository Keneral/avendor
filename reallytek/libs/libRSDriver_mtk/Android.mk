LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libRSDriver_mtk
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libRS libRS_internal libcrypto libEGL libGLESv1_CM libGLESv2 libui libgui libsync libbcc libbcinfo libLLVM libOpenCLIcd libc++ libblas libz
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/libRSDriver_mtk.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = libRSDriver_mtk
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libRS libRS_internal libcrypto libEGL libGLESv1_CM libGLESv2 libui libgui libsync libbcc libbcinfo libLLVM libOpenCLIcd libc++ libblas libz
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/libRSDriver_mtk.so
include $(BUILD_PREBUILT)
