LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = wpa_supplicant_overlay.conf
LOCAL_MODULE_CLASS = ETC
LOCAL_MODULE_OWNER = mtk
LOCAL_MODULE_PATH = $(PRODUCT_OUT)/system/etc/wifi
LOCAL_PROPRIETARY_MODULE = true
LOCAL_SRC_FILES = arm/wpa_supplicant_overlay.conf
include $(BUILD_PREBUILT)
