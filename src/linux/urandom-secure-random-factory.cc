/**
 * @file	urandom-secure-random-factory.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/secure-random-factory.h"

#include <windows.h>
#include <wincrypt.h>

namespace jcu {
    namespace random {

        namespace linux {

            class UrandomSecureRandom : public SecureRandom {
            private:
                int fd_;

            public:
                WinSecureRandom() {
                    fd_ = open("/dev/urandom", O_RDONLY);
                }

                ~WinSecureRandom() override {
                    if(fd_ != -1) {
                        close(fd_);
                        fd_ = -1;
                    }
                    if(crypt_prov_handler_) {
                        ::CryptReleaseContext(crypt_prov_handler_, 0);
                        crypt_prov_handler_ = NULL;
                    }
                }

                int next(int bits) override {
                    uint64_t temp = 0;
                    if(fd_ != -1) {
                        read(fd_, &temp, sizeof(temp));
                    }
                    temp &= JCU_RANDOM_NEXT_MASK;
                    return (int)(temp >> ((unsigned)(48 - bits)));
                }

                void setSeed(uint64_t seed) override {
                    // Ignore
                }
            };

            class UrandomSecureRandomFactory : public SecureRandomFactory {
            public:
                std::unique_ptr<SecureRandom> create() override {
                    return std::unique_ptr<UrandomSecureRandom>(new UrandomSecureRandom());
                }
            };

        }

        SecureRandomFactory* getSecureRandomFactory() {
            static std::unique_ptr<linux::UrandomSecureRandomFactory> instance(new linux::UrandomSecureRandomFactory());
            return instance.get();
        }

    }
}
