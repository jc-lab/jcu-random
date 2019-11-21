/**
 * @file	secure-random.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_RANDOM_SECURE_RANDOM_H__
#define __JCU_RANDOM_SECURE_RANDOM_H__

#include "random.h"

namespace jcu {
    namespace random {

        class SecureRandom : public Random {
            virtual void getSeed(void *buf, size_t length) = 0;
        };

    }
}

#endif //__JCU_RANDOM_SECURE_RANDOM_H__
