/**
 * @file	well512-random.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/well512-random.h"
#include "random-intl.h"

namespace jcu {
namespace random {

#define W 32
#define P 0
#define M1 13
#define M2 9
#define M3 5

#define MAT0POS(t, v) (v^(v>>t))
#define MAT0NEG(t, v) (v^(v<<(-(t))))
#define MAT3NEG(t, v) (v<<(-(t)))
#define MAT4NEG(t, b, v) (v ^ ((v<<(-(t))) & b))

#define V0            state_data_[state_i_                   ]
#define VM1           state_data_[(state_i_+M1) & 0x0000000fU]
#define VM2           state_data_[(state_i_+M2) & 0x0000000fU]
#define VM3           state_data_[(state_i_+M3) & 0x0000000fU]
#define VRm1          state_data_[(state_i_+15) & 0x0000000fU]
#define VRm2          state_data_[(state_i_+14) & 0x0000000fU]
#define newV0         state_data_[(state_i_+15) & 0x0000000fU]
#define newV1         state_data_[state_i_                   ]
#define newVRm1       state_data_[(state_i_+14) & 0x0000000fU]

Well512Random::Well512Random() {
  setSeed(getTempSeed());
}

Well512Random::~Well512Random() {
  state_i_ = 0;
  for (int i = 0; i < 16; i++) {
    state_data_[i] = 0;
  }
}

uint32_t Well512Random::wnext() {
  uint32_t z0 = VRm1;
  uint32_t z1 = MAT0NEG (-16, V0) ^MAT0NEG (-15, VM1);
  uint32_t z2 = MAT0POS (11, VM2);
  newV1 = z1 ^ z2;
  newV0 = MAT0NEG (-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
  state_i_ = (state_i_ + 15) & 0xfU;
  return state_data_[state_i_];
}

int Well512Random::next(int bits) {
  uint64_t temp = ((uint64_t) wnext()) << 32u | ((uint64_t) wnext());
  temp &= JCU_RANDOM_NEXT_MASK;
  return (int) (temp >> ((unsigned) (48 - bits)));
}

void Well512Random::setSeed(uint64_t seed) {
  int i;
  uint32_t temp;
  state_i_ = 0;
  state_data_[0] = (uint32_t) (seed >> 32u);
  state_data_[1] = (uint32_t) seed;

  temp = (uint32_t) ((state_data_[0] * 0x5DEECE66D) + state_data_[1]);

  for (i = 2; i < 16; i++) {
    state_data_[i] = temp = (uint32_t) (temp * 0x5DEECE66DL + 0xBL);
  }
}

} // namespace random
} // namespace jcu
