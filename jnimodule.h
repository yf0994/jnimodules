/*
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

#ifndef __JNIMODULE_H__
#define __JNIMODULE_H__

#include "jni.h"
#include "log.h"

namespace jnimodule
{
	class JNIModule
	{
	public:
		JNIModule() {}
		~JNIModule() {}
		void onLoad(JavaVM* vm, JNIEnv* env, void* reserved);
		static jstring Runtime_nativeLoad(JNIEnv* env, jclass, jstring javaFilename, jobject javaLoader, jstring javaLdLibraryPath);
	};
}

#endif 
