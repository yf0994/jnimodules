LOCAL_PATH :=$(call my-dir)
$(info $(LOCAL_PATH))

MY_LOCAL_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := jnihelper

LOCAL_SRC_FILES := $(MY_LOCAL_PATH)/android-libs/armeabi/libnativehelper.so 

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := aes

LOCAL_SRC_FILES := $(MY_LOCAL_PATH)/android-libs/armeabi/libcrypto.so

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := encryptio

LOCAL_MODULE_FILENAME := libencryptio

LOCAL_SRC_FILES := Posix.cpp \
	internal_constants.cpp \
	encryptio.cpp \
	Cache.cpp \
	aes_api.cpp \
	File.cpp \
	Encrypt.cpp \

LOCAL_C_INCLUDES := $(MY_LOCAL_PATH) \
	$(MY_LOCAL_PATH)/jniutils \
	$(MY_LOCAL_PATH)/openssl \

#LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/jniutils

LOCAL_LDLIBS := -llog \

LOCAL_SHARED_LIBRARIES := jnihelper \
	aes


include $(BUILD_SHARED_LIBRARY)

