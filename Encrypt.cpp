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


#include "Encrypt.h"

namespace AES
{
    int encrypted(const char* data, int length, Cache* cache)
    {
        int reminder = length % 16;
        int quotient = length / 16;
        
        int n = reminder == 0 ? quotient : quotient + 1 , i, index;
        index = 0;
        for(i = 0; i < n; i++)
        {
            char buffer[LEN];
            memcpy(buffer, data + index, LEN);
            Api::encrypts(buffer, buffer);
            cache -> add(buffer);
            index += 16;
        }

        return index;
    }
    
    int decrypted(int fd, char* data, int length)
    {
        int ret = 0;
        int index = 0;
        char buffers[LEN];
        while(index < length && (ret = read(fd, buffers, LEN)) > 0)
        {
            Api::decrypts(buffers, buffers);
            memcpy(data + index, buffers, LEN);
            index += LEN;

        }
        return index;
    }
}