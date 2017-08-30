LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = audio.primary.mt6737m
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_RELATIVE_PATH = hw
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES_64 = libmedia libhardware_legacy libhardware libaudiocustparam libaudioutils libtinyalsa libtinycompress libcustom_nvram libtinyxml libaudiotoolkit libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = arm64/audio.primary.mt6737m.so
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE = audio.primary.mt6737m
LOCAL_MODULE_CLASS = SHARED_LIBRARIES
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_RELATIVE_PATH = hw
LOCAL_MODULE_SUFFIX = .so
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libmedia libhardware_legacy libhardware libaudiocustparam libaudioutils libtinyalsa libtinycompress libcustom_nvram libtinyxml libaudiotoolkit libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/audio.primary.mt6737m.so
include $(BUILD_PREBUILT)
