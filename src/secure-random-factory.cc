/**
 * @file	secure-random-factory.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/secure-random-factory.h"
#include "jcu-random/random-provider.h"

#include "digest/sha1_digest.h"

#include <mutex>
#include <iostream>

namespace jcu {
    namespace random {

        template<class TDIGEST, int DIGEST_SIZE>
        class DigestSecureRandom : public SecureRandom {
        private:
            std::mutex mutex_;

            Sha1Digest digest_;

            uint64_t state_counter_;
            unsigned char state_[DIGEST_SIZE];
            unsigned char seed_[DIGEST_SIZE];

        public:
            DigestSecureRandom()
            : state_counter_(0) {
                int i;
                for(i=0; i<DIGEST_SIZE; i++) {
                    seed_[i] = 0;
                }
                for(i=0; i<DIGEST_SIZE; i++) {
                    state_[i] = 0;
                }
            }

            int next(int bits) override {
                uint64_t temp = 0;
                nextBytes(&temp, sizeof(uint64_t));
                temp &= JCU_RANDOM_NEXT_MASK;
                return (int)(temp >> ((unsigned)(48 - bits)));
            }
            void setSeed(const void *data, int length) {
                std::unique_lock<std::mutex> lock(mutex_);
                digest_.update(&seed_, sizeof(seed_));
                digest_.update(data, length);
                digest_.final(seed_);
            }
            void setSeed(uint64_t seed) override {
                setSeed(&seed, sizeof(seed));
            }
            void cycleSeed() {
                std::unique_lock<std::mutex> lock(mutex_);

                digest_.update(seed_, sizeof(seed_));
                digest_.update(&state_counter_, sizeof(state_counter_));
                digest_.final(seed_);
            }
            void generateState() {
                {
                    std::unique_lock<std::mutex> lock(mutex_);

                    digest_.update(&state_counter_, sizeof(state_counter_));
                    state_counter_++;

                    digest_.update(state_, DIGEST_SIZE);
                    digest_.update(seed_, DIGEST_SIZE);

                    digest_.final(state_);
                }

                if ((state_counter_ % 10) == 0)
                {
                    cycleSeed();
                }
            }
            void nextBytes(void *buf, size_t length) override {
                unsigned char *bytes = (unsigned char*)buf;
                int state_off = 0;

                generateState();

                for (size_t i = 0; i < length; i++)
                {
                    if (state_off == DIGEST_SIZE)
                    {
                        generateState();
                        state_off = 0;
                    }
                    bytes[i] = state_[state_off++];
                }
            }

        };

        class Sha1PrngSecureFactory : public SecureRandomFactory {
        public:
            std::unique_ptr<SecureRandom> create() override {
                std::unique_ptr<RandomProvider> provider(getSystemRandomProvider());
                std::unique_ptr<DigestSecureRandom<Sha1Digest, 20>> inst(new DigestSecureRandom<Sha1Digest, 20>());
                unsigned char buf[128];
                provider->nextBytes(buf, sizeof(buf));
                inst->setSeed(buf, sizeof(buf));
                return std::move(inst);
            }
        };

        SecureRandomFactory *getSecureRandomFactory() {
            static std::unique_ptr<SecureRandomFactory> instance(new Sha1PrngSecureFactory());
            return instance.get();
        }
    }
}
