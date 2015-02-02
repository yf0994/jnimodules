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

#include "Cache.h"

namespace AES
{
    Cache::Cache(int fd) : size(0) , fd(fd)
    {
        cache = (char*)malloc(MAX_LENGTH);
        if(cache == NULL)
        {
            return;
        }
        memset(cache, 0, MAX_LENGTH);
    }
    
    Cache::~Cache()
    {
        free(cache);
        cache = NULL;
    }
    
    void Cache::setFunc(func _func)
    {
        method = _func;
    }
    
    void Cache::add(char* data)
    {
        memcpy(cache + size * 16, data, LENGTH);
        size++;
        if(size == COUNT)
        {
            flush();
            
        }
    }
    
    void Cache::flush()
    {
        if(method == write)
        {
            method(fd, cache, size * 16);
            memset(cache, 0, MAX_LENGTH);
            size = 0;
        }
        
    }
}