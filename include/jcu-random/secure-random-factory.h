/**
 * @file	secure-random-factory.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_RANDOM_SECURE_RANDOM_FACTORY_H__
#define __JCU_RANDOM_SECURE_RANDOM_FACTORY_H__

#include <memory>

#include "secure-random.h"

namespace jcu {
    namespace random {

        class SecureRandomFactory {
        public:
            virtual std::unique_ptr<SecureRandom> create() = 0;
        };

        extern SecureRandomFactory* getSecureRandomFactory();

    }
}

#endif //__JCU_RANDOM_SECURE_RANDOM_FACTORY_H__
