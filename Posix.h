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

 #ifndef __POSIX_H__
 #define __POSIX_H__

#include "JniConstants.h" 
#include "jni.h"
#include "log.h"

namespace encryptio
{
	class Posix
	{
	public:
		static jobject Posix_open(JNIEnv* env, jobject, jstring javaPath, jint flags, jint mode);

		static void Posix_close(JNIEnv* env, jobject, jobject javaFd);

		static jobject Posix_stat(JNIEnv* env, jobject, jstring javaPath);

		static jobject Posix_fstat(JNIEnv* env, jobject, jobject javaFd);

		static jint Posix_preadBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes, jint byteOffset, jint byteCount, jlong offset);

		static jint Posix_pwriteBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes, jint byteOffset, jint byteCount, jlong offset);

		static jint Posix_readBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes, jint byteOffset, jint byteCount);

		static jint Posix_writeBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes, jint byteOffset, jint byteCount);

		static jlong Posix_mmap(JNIEnv* env, jobject, jlong address, jlong byteCount, jint prot, jint flags, jobject javaFd, jlong offset);

		static void Posix_munmap(JNIEnv* env, jobject, jlong address, jlong byteCount);

		static void Posix_remove(JNIEnv* env, jobject, jstring javaPath);

		static void Posix_rename(JNIEnv* env, jobject, jstring javaOldPath, jstring javaNewPath);

		static jint Posix_ioctlInt(JNIEnv* env, jobject, jobject javaFd, jint cmd, jobject javaArg);

		static void Posix_ftruncate(JNIEnv* env, jobject, jobject javaFd, jlong length);

		static jlong Posix_lseek(JNIEnv* env, jobject, jobject javaFd, jlong offset, jint whence);

		void register_libcore_io_Posix(JNIEnv* env);
	};
}

 #endif
