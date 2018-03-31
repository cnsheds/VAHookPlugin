LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= helloJni.cpp

LOCAL_LDLIBS    := -llog

LOCAL_MODULE:= helloJni

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive
LOCAL_CFLAGS += -fno-rtti -fexceptions

include $(BUILD_SHARED_LIBRARY)
