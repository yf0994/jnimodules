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

#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "Cache.h"

#define MAGIC_DIGIT 0x0ea8b311

using std::list;
using AES::Cache;
typedef struct FileHead
{
    int MAGIC;
}FileHead;

class FileInfo
{
public:
    FileInfo(int fd) : fd(fd), _isEncrypt(false) { cache = new Cache(fd); };
    ~FileInfo() { delete cache; };
    bool isEncrypt() const { return _isEncrypt; }
    bool readHead();
    void setFileInfo();
    int getFd () const { return fd; }
    Cache* getCache() const { return cache; }
    
private:
    int fd;
    bool _isEncrypt;
    Cache* cache;
    
};


FileInfo* getFileInfo(int fd);

void removeFileInfo(FileInfo* info);

void addFileInfo(FileInfo* info);

#endif
