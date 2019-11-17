/**
 * @file	win-random-provider.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/random-provider.h"

#include <windows.h>
#include <wincrypt.h>

namespace jcu {
    namespace random {

        namespace platform_windows {

            class WinCryptRandomProvider : public RandomProvider {
            private:
                HCRYPTPROV crypt_prov_handler_;

            public:
                WinCryptRandomProvider() {
                    crypt_prov_handler_ = NULL;
                    if(!::CryptAcquireContext(&crypt_prov_handler_, NULL, NULL, PROV_RSA_FULL, 0)) {
                        crypt_prov_handler_ = NULL;
                    }
                }

                ~WinCryptRandomProvider() override {
                    if(crypt_prov_handler_) {
                        ::CryptReleaseContext(crypt_prov_handler_, 0);
                        crypt_prov_handler_ = NULL;
                    }
                }

                int nextBytes(unsigned char *buffer, int length) override {
                    ::CryptGenRandom(crypt_prov_handler_, length, (BYTE *)buffer);
                    return length;
                }
            };

        }

        std::unique_ptr<RandomProvider> getSystemRandomProvider() {
            return std::unique_ptr<platform_windows::WinCryptRandomProvider>(new platform_windows::WinCryptRandomProvider());
        }

    }
}
