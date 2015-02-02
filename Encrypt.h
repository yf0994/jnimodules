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


#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include <stdio.h>
#include "Cache.h"
#include "aes_api.h"

#define LEN 16

using AES::Cache;
namespace AES
{
    int encrypted(const char* data, int length, Cache* cache);

    int decrypted(int fd, char* data, int length);
}

#endif 
