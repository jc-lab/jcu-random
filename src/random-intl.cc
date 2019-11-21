/**
 * @file	random-intl.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "random-intl.h"

#include <config-intl.h>

#include <atomic>

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_GETTICKCOUNT
#include <windows.h>
#endif
#ifdef HAVE_GETTIMEOFDAY
#include <sys/time.h>
#endif

namespace jcu {
    namespace random {

        static std::atomic<uint64_t> old_seed_(1);

        static void addSeed(uint64_t *target, uint64_t value) {
            uint64_t out = *target * 0x5DEECE66DULL;
            out += value;
            *target = out;
        }

        uint64_t getTempSeed() {
            uint64_t seed = old_seed_.load();
#ifdef HAVE_TIME_H
            addSeed(&seed, time(NULL));
#endif
#ifdef HAVE_GETTICKCOUNT
            addSeed(&seed, ::GetTickCount());
#endif
#ifdef HAVE_GETTIMEOFDAY
            {
                timeval tv = {0, 0};
                ::gettimeofday(&tv, NULL);
                addSeed(&seed, tv.tv_sec);
                addSeed(&seed, tv.tv_usec);
            }
#endif

            old_seed_.store(seed);
            return seed;
        }

    }
}
