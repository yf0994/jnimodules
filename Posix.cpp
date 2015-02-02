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

#define LOG_TAG "Posix"


#include "Posix.h"
#include "JNIHelp.h"
#include "ScopedBytes.h"
#include "ScopedLocalRef.h"
#include "ScopedUtfChars.h"
#include "File.h"
#include "Encrypt.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
using encryptio::Posix;
using AES::Cache;
using namespace AES;

 namespace encryptio
 {

 	static jobject makeStructStat(JNIEnv* env, const struct stat& sb)
 	{
 		static jmethodID ctor = env -> GetMethodID(JniConstants::structStatClass, "<init>",
 			"(JJIJIIJJJJJJJ)V");
 		return env -> NewObject(JniConstants::structStatClass, ctor,
 				static_cast<jlong>(sb.st_dev), static_cast<jlong>(sb.st_ino),
 				static_cast<jint>(sb.st_mode), static_cast<jlong>(sb.st_nlink),
 				static_cast<jint>(sb.st_uid), static_cast<jint>(sb.st_gid),
 				static_cast<jlong>(sb.st_rdev), static_cast<jlong>(sb.st_size),
 				static_cast<jlong>(sb.st_atime), static_cast<jlong>(sb.st_mtime),
 				static_cast<jlong>(sb.st_ctime), static_cast<jlong>(sb.st_blksize),
 				static_cast<jlong>(sb.st_blocks));
 	}
 	
 	static void throwException(JNIEnv* env, jclass exceptionClass, jmethodID ctor3, jmethodID ctor2,
 		const char* functionName, int error)
 	{
 		jthrowable cause = NULL;
 		if(env -> ExceptionCheck())
 		{
 			cause = env -> ExceptionOccurred();
 			env -> ExceptionClear();
 		}

 		ScopedLocalRef<jstring> detailMessage(env, env -> NewStringUTF(functionName));
 		if(detailMessage.get() == NULL)
 		{
 			env -> ExceptionClear();
 		}

 		jobject exception;
 		if(cause != NULL)
 		{
 			exception = env -> NewObject(exceptionClass, ctor3, detailMessage.get(), error, cause);
 		}
 		else
 		{
 			exception = env -> NewObject(exceptionClass, ctor2, detailMessage.get(), error);
 		}

 		env -> Throw(reinterpret_cast<jthrowable>(exception));
 	}


 	static void throwErrnoException(JNIEnv* env, const char* functionName)
 	{
 		int error = errno;
 		static jmethodID ctor3 = env -> GetMethodID(JniConstants::errnoExceptionClass,
 				"<init>", "(Ljava/lang/String;ILjava/lang/Throwable;)V");
 		static jmethodID ctor2 = env -> GetMethodID(JniConstants::errnoExceptionClass,
 				"<init>", "(Ljava/lang/String;I)V");
 		throwException(env, JniConstants::errnoExceptionClass, ctor3, ctor2, functionName, error);
 	}

 	template <typename rc_t>
 	static rc_t throwIfMinusOne(JNIEnv* env, const char* name, rc_t rc)
 	{
 		if(rc == rc_t(-1))
 		{
 			throwErrnoException(env, name);
 		}
 		return rc;
 	}

 	static jobject doStat(JNIEnv* env, jstring javaPath, bool isLast)
 	{
 		ScopedUtfChars path(env, javaPath);
 		if(path.c_str() == NULL)
 		{
 			return NULL;
 		}

 		struct stat sb;
 		int rc = isLast ? TEMP_FAILURE_RETRY(lstat(path.c_str(), &sb)) 
 						: TEMP_FAILURE_RETRY(stat(path.c_str(), &sb));
 		if(rc == -1)
 		{
 			throwErrnoException(env, isLast ? "lstat" : "stat");
 			return NULL;
 		}
 		return makeStructStat(env, sb);
 	}

 	static JNINativeMethod gMethod[]=
	{
		CPP_NATIVE_METHOD(open, "(Ljava/lang/String;II)Ljava/io/FileDescriptor;", Posix::Posix_open),
		CPP_NATIVE_METHOD(close, "(Ljava/io/FileDescriptor;)V", Posix::Posix_close),
		CPP_NATIVE_METHOD(fstat, "(Ljava/io/FileDescriptor;)Landroid/system/StructStat;", Posix::Posix_fstat),
		CPP_NATIVE_METHOD(stat, "(Ljava/lang/String;)Landroid/system/StructStat;", Posix::Posix_stat),
		CPP_NATIVE_METHOD(preadBytes, "(Ljava/io/FileDescriptor;Ljava/lang/Object;IIJ)I", Posix::Posix_preadBytes),
		CPP_NATIVE_METHOD(pwriteBytes, "(Ljava/io/FileDescriptor;Ljava/lang/Object;IIJ)I", Posix::Posix_pwriteBytes),
		CPP_NATIVE_METHOD(readBytes, "(Ljava/io/FileDescriptor;Ljava/lang/Object;II)I", Posix::Posix_readBytes),
		CPP_NATIVE_METHOD(writeBytes, "(Ljava/io/FileDescriptor;Ljava/lang/Object;II)I", Posix::Posix_writeBytes),
		CPP_NATIVE_METHOD(mmap, "(JJIILjava/io/FileDescriptor;J)J", Posix::Posix_mmap),
		CPP_NATIVE_METHOD(munmap, "(JJ)V", Posix::Posix_munmap),
		CPP_NATIVE_METHOD(remove, "(Ljava/lang/String;)V", Posix::Posix_remove),
		CPP_NATIVE_METHOD(rename, "(Ljava/lang/String;Ljava/lang/String;)V", Posix::Posix_rename),
		CPP_NATIVE_METHOD(lseek, "(Ljava/io/FileDescriptor;JI)J", Posix::Posix_lseek),
		CPP_NATIVE_METHOD(ioctlInt, "(Ljava/io/FileDescriptor;ILandroid/util/MutableInt;)I", Posix::Posix_ioctlInt),
		CPP_NATIVE_METHOD(ftruncate, "(Ljava/io/FileDescriptor;J)V", Posix::Posix_ftruncate)
	};

 	jobject Posix::Posix_open(JNIEnv* env, jobject, jstring javaPath, jint flags, jint mode)
 	{
 		ScopedUtfChars path(env, javaPath);
 		LOG_I("%s\n", path.c_str());
 		if(path.c_str() == NULL)
 			return NULL;
 		int fd = throwIfMinusOne(env, "open", TEMP_FAILURE_RETRY(open(path.c_str(),flags, mode)));
 		if(fd != -1)
 		{
 			FileInfo* info = getFileInfo(fd);
 			if(info == NULL)
 			{
 				FileInfo* fileinfo = new FileInfo(fd);
 				addFileInfo(fileinfo);
 			}
 		}
 		return fd != -1 ? jniCreateFileDescriptor(env, fd) : NULL;
 	}

	void Posix::Posix_close(JNIEnv* env, jobject, jobject javaFd)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		jniSetFileDescriptorOfFD(env, javaFd, -1);
		FileInfo* info = getFileInfo(fd);
		if(info != NULL)
		{
			info -> getCache() -> flush();
			removeFileInfo(info);
			delete info;
		}
		throwIfMinusOne(env, "close", close(fd));
	}

	jobject Posix::Posix_stat(JNIEnv* env, jobject, jstring javaPath)
	{
		return doStat(env, javaPath, false);
	}

	jobject Posix::Posix_fstat(JNIEnv* env, jobject, jobject javaFd)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		struct stat sb;
		int rc = TEMP_FAILURE_RETRY(fstat(fd, &sb));
		if(rc == -1)
		{
			throwErrnoException(env, "fstat");
			return NULL;
		}
		return makeStructStat(env, sb);
	}

	jint Posix::Posix_preadBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes, jint byteOffset, jint byteCount, jlong offset)
	{
		return 0;
	}

	jint Posix::Posix_pwriteBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes, jint byteOffset, jint byteCount, jlong offset)
	{
		return 0;
	}

	jint Posix::Posix_readBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes, jint byteOffset, jint byteCount)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		ScopedBytesRW bytes(env, javaBytes);
		if(bytes.get() == NULL)
		{
			return -1;
		}
		FileInfo* info = getFileInfo(fd);
		if(info == NULL)
		{
			return throwIfMinusOne(env, "readBytes", TEMP_FAILURE_RETRY(read(fd, bytes.get() + byteOffset, byteCount)));
		}
		else
		{
			return throwIfMinusOne(env, "readBytes", TEMP_FAILURE_RETRY(decrypted(fd, (char *)bytes.get() + byteOffset, byteCount)));
		}
		
	}

	jint Posix::Posix_writeBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes, jint byteOffset, jint byteCount)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		ScopedBytesRO bytes(env, javaBytes);
		if(bytes.get() == NULL)
		{	
			return -1;
		}
		FileInfo* info = getFileInfo(fd);
		if(info == NULL)
		{
			return throwIfMinusOne(env, "writeBytes", TEMP_FAILURE_RETRY(write(fd, bytes.get() + byteOffset, byteCount)));
		}
		else
		{
			AES::Cache* cache = info -> getCache();
			cache -> setFunc(write);
			return throwIfMinusOne(env, "writeBytes", TEMP_FAILURE_RETRY(encrypted((const char*)bytes.get() + byteOffset, byteCount, cache)));
		}
		
	}

	jlong Posix::Posix_mmap(JNIEnv* env, jobject, jlong address, jlong byteCount, jint prot, jint flags, jobject javaFd, jlong offset)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		void* suggestedPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
		void* ptr = mmap(suggestedPtr, byteCount, prot, flags, fd, offset);
		if(ptr == MAP_FAILED)
		{
			throwErrnoException(env, "mmap");
		}
		return static_cast<jlong>(reinterpret_cast<uintptr_t>(ptr));
	}

	void Posix::Posix_munmap(JNIEnv* env, jobject, jlong address, jlong byteCount)
	{
		void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
		throwIfMinusOne(env, "munmap", TEMP_FAILURE_RETRY(munmap(ptr, byteCount)));
	}

	void Posix::Posix_remove(JNIEnv* env, jobject, jstring javaPath)
	{
		ScopedUtfChars path(env, javaPath);
		if(path.c_str() == NULL)
		{
			return;
		}
		throwIfMinusOne(env, "remove", TEMP_FAILURE_RETRY(remove(path.c_str())));

	}

	void Posix::Posix_rename(JNIEnv* env, jobject, jstring javaOldPath, jstring javaNewPath)
	{
		ScopedUtfChars oldPath(env, javaOldPath);
		if(oldPath.c_str() == NULL)
		{
			return;
		}

		ScopedUtfChars newPath(env, javaNewPath);
		if(newPath.c_str() == NULL)
		{
			return;
		}
		throwIfMinusOne(env, "rename", TEMP_FAILURE_RETRY(rename(oldPath.c_str(), newPath.c_str())));
	}

	jint Posix::Posix_ioctlInt(JNIEnv* env, jobject, jobject javaFd, jint cmd, jobject javaArg)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		static jfieldID valueFid = env -> GetFieldID(JniConstants::mutableIntClass, "value", "I");
		jint arg = env -> GetIntField(javaArg, valueFid);
		int rc = throwIfMinusOne(env, "ioctl", TEMP_FAILURE_RETRY(ioctl(fd, cmd, &arg)));
		if(!env -> ExceptionCheck())
		{
			env -> SetIntField(javaArg, valueFid, arg);
		}
		return rc;
	}

	void Posix::Posix_ftruncate(JNIEnv* env, jobject, jobject javaFd, jlong length)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		throwIfMinusOne(env, "ftruncate", TEMP_FAILURE_RETRY(ftruncate(fd, length)));
	}

	jlong Posix::Posix_lseek(JNIEnv* env, jobject, jobject javaFd, jlong offset, jint whence)
	{
		int fd = jniGetFDFromFileDescriptor(env, javaFd);
		return throwIfMinusOne(env, "lseek", TEMP_FAILURE_RETRY(lseek64(fd, offset, whence)));
	}

 	void Posix::register_libcore_io_Posix(JNIEnv* env)
 	{
 		LOG_I("register_libcore_io_Posix");
 		int ret = jniRegisterNativeMethods(env, "libcore/io/Posix", gMethod, NELEM(gMethod));
 		LOG_E("jniRegisterNativeMethods %d\n", ret);
 		LOG_I("register_libcore_io_Posix end");
 	}
 }

