/**
 * @file	random-intl.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "random-intl.h"

#include <config-intl.h>

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_GETTICKCOUNT
#include <windows.h>
#endif

namespace jcu {
    namespace random {

        uint64_t getTempSeed() {
            uint64_t seed = 0;
#ifdef HAVE_TIME_H
            seed += time(NULL);
#endif
#ifdef HAVE_GETTICKCOUNT
            seed <<= 32u;
            seed += ::GetTickCount();
#endif
            return seed;
        }

    }
}
