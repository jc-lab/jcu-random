/**
 * @file	random.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/random.h"

namespace jcu {
    namespace random {

        static bool _imin(int a, int b) {
            return (a < b) ? a : b;
        }

        void Random::nextBytes(void *buf, size_t length) {
            unsigned char *buf_ptr = (unsigned char *)buf;
            for (size_t i = 0; i < length; ) {
                for (unsigned int rnd = nextInt(), n = _imin(length - i, 4);
                     n-- > 0; rnd >>= 8u) {
                    buf_ptr[i++] = (unsigned char) rnd;
                }
            }
        }
        bool Random::nextBoolean() {
            return next(1) != 0;
        }
        double Random::nextDouble() {
            return ((double)((((uint64_t)next(26)) << 27u) + (int64_t)next(27)))
                / (double)(1LLu << 53u);
        }
        float Random::nextFloat() {
            return ((float)next(24)) / ((float)(1u << 24u));
        }
        int Random::nextInt(int bound) {
            if (bound > 0) {
                if ((((unsigned) bound) & ((unsigned) (-bound))) == bound)  // i.e., bound is a power of 2
                    return (int) (((uint64_t) bound * (uint64_t) next(31)) >> 31u);

                int bits, val;
                do {
                    bits = next(31);
                    val = bits % bound;
                } while (bits - val + (bound - 1) < 0);
                return val;
            }
            return next(31);
        }
        int32_t Random::nextInt32(int bound) {
            return nextInt(bound);
        }
        int64_t Random::nextInt64() {
            return ((uint64_t)next(32) << 32u) + next(32);
        }
    }
}
