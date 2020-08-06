/**
 * @file	sha1-digest.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/17
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __DIGEST_SHA1_DIGEST_H__
#define __DIGEST_SHA1_DIGEST_H__

#include "digest.h"

#include <stdint.h>

namespace jcu {
namespace random {

class Sha1Digest : public Digest {
 private:
  uint32_t state_[5];
  unsigned char buffer_[64];
  int buffer_pos_;
  uint64_t transforms_;

 public:
  Sha1Digest();

  int getDigestSize() override;

  void reset() override;
  void update(const void *data, int length) override;
  void final(void *data) override;
};

} // namespace random
} // namespace jcu

#endif //__DIGEST_DIGEST_H__
