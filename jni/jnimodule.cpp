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


// #include ""
#include "jnimodule.h"
#include "ScopedUtfChars.h"
#include "jniConstants.h"


#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
namespace jnimodule
{

static jstring Runtime_nativeLoad(JNIEnv* env, jclass, jstring javaFilename, jobject javaLoader, jstring javaLdLibraryPath) {
  	ScopedUtfChars filename(env, javaFilename);
  	if (filename.c_str() == NULL) {
    	return NULL;
  	}
  	LOG_I("filename:%s\n",filename.c_str());
  	return NULL;
  }
/*
说明 : native函数列表
*/
static JNINativeMethod gMethod[] = 
{
	/*
	说明：第一个参数是Java层的函数名，第二个参数是函数的签名，第三个参数是本地方法的地址
	*/
 	CPP_NATIVE_METHOD(nativeLoad, "(Ljava/lang/String;Ljava/lang/ClassLoader;Ljava/lang/String;)Ljava/lang/String;", (void*)Runtime_nativeLoad)
};

void JNIModule::onLoad(JavaVM* vm, JNIEnv* env, void* reserved)
{
	/*
	说明：第二个参数是Java中的包名＋类名
	*/
 	jniRegisterNativeMethods(env, "java/lang/Runtime", gMethod, NELEM(gMethod));
}
}


 jint JNI_OnLoad(JavaVM* vm, void* reserved)
 {
    JNIEnv* env = NULL;
 	  if(vm -> GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
 	  {
 		   LOG_E("JavaVM::GetEnv() failed");
 		   abort();
 	  }

 	  jnimodule::JNIModule* module = new jnimodule::JNIModule;
 	  module -> onLoad(vm, env, reserved);
 	  delete module;
 	  return JNI_VERSION_1_6;
}
