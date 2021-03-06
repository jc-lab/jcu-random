/**
 * @file	digest.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/17
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __DIGEST_DIGEST_H__
#define __DIGEST_DIGEST_H__

#include <stdint.h>

namespace jcu {
namespace random {

class Digest {
 public:
  virtual ~Digest() {};

  virtual int getDigestSize() = 0;

  virtual void reset() = 0;
  virtual void update(const void *data, int length) = 0;
  virtual void final(void *data) = 0;
};

} // namespace random
} // namespace jcu

#endif //__DIGEST_DIGEST_H__
