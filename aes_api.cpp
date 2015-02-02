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

#include "aes_api.h"
namespace AES
{
    
    bool Api::initKey(UserKey *key)
    {
        if (key == NULL)
            return false;
        for (int i = 0; i < AES_BLOCK_SIZE; ++i)
            key -> data[i] = 32 + i;
        return true;
    }
    
    bool Api::encrypts(const char *in,char *out)
    {
        UserKey uk;
        AES_KEY ak;
        initKey(&uk);
        AES_set_encrypt_key(uk.data, 128, &ak);
        AES_encrypt((unsigned char *)in, (unsigned char *)out, &ak);
        return true;
    }
    
    bool Api::decrypts(const char *in, char *out)
    {
        UserKey uk;
        AES_KEY ak;
        initKey(&uk);
        AES_set_decrypt_key(uk.data, 128, &ak);
        AES_decrypt((unsigned char *)in, (unsigned char *)out, &ak);
        return true;
    }
    
}
