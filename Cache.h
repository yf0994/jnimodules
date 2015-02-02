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

#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


#define MAX_LENGTH 4096
#define LENGTH 16
#define COUNT 256

namespace AES {
    class Cache
    {
    public:
        typedef ssize_t (*func)(int fd, const void* buffer, size_t count);
        Cache(int fd);
        ~Cache();
        void add(char* data);
        void flush();
        void setFunc(func _func);
    private:
        char* cache;
        int size;
        int fd;
        func method;
    };
}

#endif
