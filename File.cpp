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

#include "File.h"

static list<FileInfo*> FileList;

void addFileInfo(FileInfo* info)
{
    if(info != NULL)
        FileList.push_back(info);
}

void removeFileInfo(FileInfo* info)
{
    if(info != NULL)
        FileList.remove(info);
}

FileInfo* getFileInfo(int fd)
{
    list<FileInfo*>::iterator iter;
    for(iter = FileList.begin(); iter != FileList.end(); ++iter)
    {
        if((*iter) -> getFd() == fd)
            return *iter;
    }
    return NULL;
}

bool FileInfo::readHead()
{
    int size = sizeof(FileHead);
    FileHead* head = (FileHead*)malloc(size);
    if(head != NULL)
    {
        int len = read(fd, head, size);
        if(len == size && head -> MAGIC == MAGIC_DIGIT)
        {
            _isEncrypt = true;
            free(head);
            return _isEncrypt;
        }
        free(head);
    }
    return false;
}
