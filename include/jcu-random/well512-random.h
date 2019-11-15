/**
 * @file	well512-random.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_RANDOM_WELL512_RANDOM_H__
#define __JCU_RANDOM_WELL512_RANDOM_H__

#include "random.h"

namespace jcu {
    namespace random {

        class Well512Random : public Random {
        private:
            uint32_t state_i_;
            uint32_t state_data_[16];

            uint32_t wnext();

        public:
            Well512Random();
            ~Well512Random();
            int next(int bits) override;
            void setSeed(uint64_t seed) override;
        };

    }
}

#endif //__JCU_RANDOM_WELL512_RANDOM_H__
