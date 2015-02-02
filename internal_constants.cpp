/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "internal_constants.h"
#include "ScopedLocalRef.h"

#include <stdlib.h>
#include <android/log.h>

#define LOG_TAG "JniConstants"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__)
jclass InternalConstants::errnoExceptionClass;
jclass InternalConstants::mutableIntClass;
jclass InternalConstants::structStatClass;

static jclass findClass(JNIEnv* env, const char* name) {
    ScopedLocalRef<jclass> localClass(env, env->FindClass(name));
    jclass result = reinterpret_cast<jclass>(env->NewGlobalRef(localClass.get()));
    if (result == NULL) {
        ALOGE("failed to find class '%s'", name);
        abort();
    }
    return result;
}

void InternalConstants::init(JNIEnv* env) {
    errnoExceptionClass = findClass(env, "android/system/ErrnoException");
    mutableIntClass = findClass(env, "android/util/MutableInt");
    structStatClass = findClass(env, "android/system/StructStat");
}
