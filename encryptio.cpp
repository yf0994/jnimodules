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

#include "encryptio.h"
#include "internal_constants.h"
#include "Posix.h"
#include <stdlib.h>
 namespace encryptio
 {
 	void EncryptIO::onLoad(JavaVM* vm, JNIEnv* env, void* reserved)
 	{
 		LOG_I("onLoad");
 		Posix* posix = new Posix;
 		posix -> register_libcore_io_Posix(env);
 		delete posix;
 		LOG_I("onLoad end");
 	}
 }


 jint JNI_OnLoad(JavaVM* vm, void* reserved)
 {
 	LOG_I("JNI_OnLoad");
 	JNIEnv* env = NULL;
 	if(vm -> GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
 	{
 		LOG_E("JavaVM::GetEnv() failed");
 		abort();
 	}

 	InternalConstants::init(env);

 	encryptio::EncryptIO* io = new encryptio::EncryptIO;
 	io -> onLoad(vm, env, reserved);
 	delete io;
 	LOG_I("JNI_OnLoad end");
 	return JNI_VERSION_1_6;
 }
