/**
 * @file	win-secure-random-factory.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/secure-random-factory.h"

#include <windows.h>
#include <wincrypt.h>

namespace jcu {
    namespace random {

        namespace windows {

            class WinSecureRandom : public SecureRandom {
            private:
                HCRYPTPROV crypt_prov_handler_;

            public:
                WinSecureRandom() {
                    crypt_prov_handler_ = NULL;
                    if(!::CryptAcquireContext(&crypt_prov_handler_, NULL, NULL, PROV_RSA_FULL, 0)) {
                        crypt_prov_handler_ = NULL;
                    }
                }

                ~WinSecureRandom() override {
                    if(crypt_prov_handler_) {
                        ::CryptReleaseContext(crypt_prov_handler_, 0);
                        crypt_prov_handler_ = NULL;
                    }
                }

                int next(int bits) override {
                    uint64_t temp = 0;
                    ::CryptGenRandom(crypt_prov_handler_, sizeof(temp), (BYTE *) &temp);
                    temp &= JCU_RANDOM_NEXT_MASK;
                    return (int)(temp >> ((unsigned)(48 - bits)));
                }

                void setSeed(uint64_t seed) override {
                    // Ignore
                }
            };

            class WinSecureRandomFactory : public SecureRandomFactory {
            public:
                std::unique_ptr<SecureRandom> create() override {
                    return std::unique_ptr<WinSecureRandom>(new WinSecureRandom());
                }
            };

        }

        SecureRandomFactory* getSecureRandomFactory() {
            static std::unique_ptr<windows::WinSecureRandomFactory> instance(new windows::WinSecureRandomFactory());
            return instance.get();
        }

    }
}
