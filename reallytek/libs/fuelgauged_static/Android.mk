LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = fuelgauged_static
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_PATH = $(PRODUCT_OUT)/root/sbin
LOCAL_MULTILIB = 32
LOCAL_SRC_FILES_32 = arm/fuelgauged_static
include $(BUILD_PREBUILT)
