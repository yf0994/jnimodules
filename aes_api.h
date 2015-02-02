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

#ifndef __AES_API_H__
#define __AES_API_H__
#include <stdio.h>
#include "openssl/aes.h"


namespace AES
{
    
    typedef struct UserKey
    {
        unsigned char data[AES_BLOCK_SIZE];
    } UserKey;
    
    class Api
    {
    public:
        static bool initKey(UserKey *uk);
        static bool encrypts(const char *in, char *out);
        static bool decrypts(const char *in,  char *out);
    };
    
}


#endif
