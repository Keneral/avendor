LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = nvram_daemon
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_OWNER = mtk
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libnvram libcustom_nvram libfile_op libnvram_daemon_callback libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/nvram_daemon
include $(BUILD_PREBUILT)
