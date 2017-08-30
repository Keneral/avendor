LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = hwcomposer.mt6735
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_RELATIVE_PATH = hw
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libui libsync libm4u libion libbwc libion_mtk libdpframework libhardware libgralloc_extra libbinder libselinux libpower libgui libged libui_ext libgui_ext libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/hwcomposer.mt6735.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = hwcomposer.mt6735
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_RELATIVE_PATH = hw
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libui libsync libm4u libion libbwc libion_mtk libdpframework libhardware libgralloc_extra libbinder libselinux libpower libgui libged libui_ext libgui_ext libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/hwcomposer.mt6735.so
include $(BUILD_PREBUILT)
