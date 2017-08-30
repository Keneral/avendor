LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = ccci_mdinit
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_OWNER = mtk
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libnvram libccci_util libhardware_legacy libmtk_drvb libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/ccci_mdinit
include $(BUILD_PREBUILT)
