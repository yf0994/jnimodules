LOCAL_PATH :=$(call my-dir)
$(info $(LOCAL_PATH))

MY_LOCAL_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := jnihelper

LOCAL_SRC_FILES := $(MY_LOCAL_PATH)/android-libs/armeabi/libnativehelper.so 

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := jnimodule

LOCAL_MODULE_FILENAME := libjnimodule

LOCAL_SRC_FILES := \
	jnimodule.cpp \

LOCAL_C_INCLUDES := $(MY_LOCAL_PATH) \
	$(MY_LOCAL_PATH)/jniutils \

	

#LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/jniutils

LOCAL_LDLIBS := -llog \

LOCAL_SHARED_LIBRARIES := jnihelper \

include $(BUILD_SHARED_LIBRARY)

