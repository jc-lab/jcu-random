/**
 * @file	random-provider.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/17
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_RANDOM_RANDOM_PROVIDER_H__
#define __JCU_RANDOM_RANDOM_PROVIDER_H__

#include <stddef.h>
#include <stdint.h>

#include <memory>

namespace jcu {
namespace random {

class RandomProvider {
 public:
  virtual ~RandomProvider() {}
  virtual int nextBytes(unsigned char *buffer, int length) = 0;
};

extern std::unique_ptr<RandomProvider> getSystemRandomProvider();

} // namespace random
} // namespace jcu

#endif //__JCU_RANDOM_RANDOM_PROVIDER_H__
