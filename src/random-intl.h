/**
 * @file	random-intl.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __RANDOM_INTL_H__
#define __RANDOM_INTL_H__

#include <stdint.h>

namespace jcu {
namespace random {

extern uint64_t getTempSeed();

} // namespace random
} // namespace jcu

#endif //__RANDOM_INTL_H__
