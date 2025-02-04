LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := EDT

LOCAL_CPPFLAGS := -std=c++17

LOCAL_SRC_FILES += main.cpp

LOCAL_CPPFLAGS += -fexceptions -frtti
include $(BUILD_EXECUTABLE)