LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = ccci_fsd
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_OWNER = mtk
LOCAL_PROPRIETARY_MODULE = true
LOCAL_MODULE_TAGS = optional
LOCAL_SHARED_LIBRARIES = libnvram libhardware_legacy libstorage_otp libc++
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/ccci_fsd
include $(BUILD_PREBUILT)
