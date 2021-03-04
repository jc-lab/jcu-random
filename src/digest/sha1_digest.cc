#include "sha1_digest.h"

#include <stddef.h>

#define DIGEST_BLOCK_INTS 16
#define DIGEST_BLOCK_BYTES (DIGEST_BLOCK_INTS * 4)

namespace jcu {
namespace random {

static uint32_t rol(const uint32_t value, const unsigned int bits) {
  return (value << bits) | (value >> (32 - bits));
}

static uint32_t blk(const uint32_t block[DIGEST_BLOCK_INTS], const unsigned int i) {
  return rol(block[(i + 13) & 15] ^ block[(i + 8) & 15] ^ block[(i + 2) & 15] ^ block[i], 1);
}

/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

static void R0(const uint32_t block[DIGEST_BLOCK_INTS],
               const uint32_t v,
               uint32_t &w,
               const uint32_t x,
               const uint32_t y,
               uint32_t &z,
               const unsigned int i) {
  z += ((w & (x ^ y)) ^ y) + block[i] + 0x5a827999 + rol(v, 5);
  w = rol(w, 30);
}

static void R1(uint32_t block[DIGEST_BLOCK_INTS],
               const uint32_t v,
               uint32_t &w,
               const uint32_t x,
               const uint32_t y,
               uint32_t &z,
               const unsigned int i) {
  block[i] = blk(block, i);
  z += ((w & (x ^ y)) ^ y) + block[i] + 0x5a827999 + rol(v, 5);
  w = rol(w, 30);
}

static void R2(uint32_t block[DIGEST_BLOCK_INTS],
               const uint32_t v,
               uint32_t &w,
               const uint32_t x,
               const uint32_t y,
               uint32_t &z,
               const unsigned int i) {
  block[i] = blk(block, i);
  z += (w ^ x ^ y) + block[i] + 0x6ed9eba1 + rol(v, 5);
  w = rol(w, 30);
}

static void R3(uint32_t block[DIGEST_BLOCK_INTS],
               const uint32_t v,
               uint32_t &w,
               const uint32_t x,
               const uint32_t y,
               uint32_t &z,
               const unsigned int i) {
  block[i] = blk(block, i);
  z += (((w | x) & y) | (w & x)) + block[i] + 0x8f1bbcdc + rol(v, 5);
  w = rol(w, 30);
}

static void R4(uint32_t block[DIGEST_BLOCK_INTS],
               const uint32_t v,
               uint32_t &w,
               const uint32_t x,
               const uint32_t y,
               uint32_t &z,
               const unsigned int i) {
  block[i] = blk(block, i);
  z += (w ^ x ^ y) + block[i] + 0xca62c1d6 + rol(v, 5);
  w = rol(w, 30);
}

/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

static void transform(uint32_t digest[], uint32_t block[DIGEST_BLOCK_INTS], uint64_t &transforms) {
  /* Copy digest[] to working vars */
  uint32_t a = digest[0];
  uint32_t b = digest[1];
  uint32_t c = digest[2];
  uint32_t d = digest[3];
  uint32_t e = digest[4];

  /* 4 rounds of 20 operations each. Loop unrolled. */
  R0(block, a, b, c, d, e, 0);
  R0(block, e, a, b, c, d, 1);
  R0(block, d, e, a, b, c, 2);
  R0(block, c, d, e, a, b, 3);
  R0(block, b, c, d, e, a, 4);
  R0(block, a, b, c, d, e, 5);
  R0(block, e, a, b, c, d, 6);
  R0(block, d, e, a, b, c, 7);
  R0(block, c, d, e, a, b, 8);
  R0(block, b, c, d, e, a, 9);
  R0(block, a, b, c, d, e, 10);
  R0(block, e, a, b, c, d, 11);
  R0(block, d, e, a, b, c, 12);
  R0(block, c, d, e, a, b, 13);
  R0(block, b, c, d, e, a, 14);
  R0(block, a, b, c, d, e, 15);
  R1(block, e, a, b, c, d, 0);
  R1(block, d, e, a, b, c, 1);
  R1(block, c, d, e, a, b, 2);
  R1(block, b, c, d, e, a, 3);
  R2(block, a, b, c, d, e, 4);
  R2(block, e, a, b, c, d, 5);
  R2(block, d, e, a, b, c, 6);
  R2(block, c, d, e, a, b, 7);
  R2(block, b, c, d, e, a, 8);
  R2(block, a, b, c, d, e, 9);
  R2(block, e, a, b, c, d, 10);
  R2(block, d, e, a, b, c, 11);
  R2(block, c, d, e, a, b, 12);
  R2(block, b, c, d, e, a, 13);
  R2(block, a, b, c, d, e, 14);
  R2(block, e, a, b, c, d, 15);
  R2(block, d, e, a, b, c, 0);
  R2(block, c, d, e, a, b, 1);
  R2(block, b, c, d, e, a, 2);
  R2(block, a, b, c, d, e, 3);
  R2(block, e, a, b, c, d, 4);
  R2(block, d, e, a, b, c, 5);
  R2(block, c, d, e, a, b, 6);
  R2(block, b, c, d, e, a, 7);
  R3(block, a, b, c, d, e, 8);
  R3(block, e, a, b, c, d, 9);
  R3(block, d, e, a, b, c, 10);
  R3(block, c, d, e, a, b, 11);
  R3(block, b, c, d, e, a, 12);
  R3(block, a, b, c, d, e, 13);
  R3(block, e, a, b, c, d, 14);
  R3(block, d, e, a, b, c, 15);
  R3(block, c, d, e, a, b, 0);
  R3(block, b, c, d, e, a, 1);
  R3(block, a, b, c, d, e, 2);
  R3(block, e, a, b, c, d, 3);
  R3(block, d, e, a, b, c, 4);
  R3(block, c, d, e, a, b, 5);
  R3(block, b, c, d, e, a, 6);
  R3(block, a, b, c, d, e, 7);
  R3(block, e, a, b, c, d, 8);
  R3(block, d, e, a, b, c, 9);
  R3(block, c, d, e, a, b, 10);
  R3(block, b, c, d, e, a, 11);
  R4(block, a, b, c, d, e, 12);
  R4(block, e, a, b, c, d, 13);
  R4(block, d, e, a, b, c, 14);
  R4(block, c, d, e, a, b, 15);
  R4(block, b, c, d, e, a, 0);
  R4(block, a, b, c, d, e, 1);
  R4(block, e, a, b, c, d, 2);
  R4(block, d, e, a, b, c, 3);
  R4(block, c, d, e, a, b, 4);
  R4(block, b, c, d, e, a, 5);
  R4(block, a, b, c, d, e, 6);
  R4(block, e, a, b, c, d, 7);
  R4(block, d, e, a, b, c, 8);
  R4(block, c, d, e, a, b, 9);
  R4(block, b, c, d, e, a, 10);
  R4(block, a, b, c, d, e, 11);
  R4(block, e, a, b, c, d, 12);
  R4(block, d, e, a, b, c, 13);
  R4(block, c, d, e, a, b, 14);
  R4(block, b, c, d, e, a, 15);

  /* Add the working vars back into digest[] */
  digest[0] += a;
  digest[1] += b;
  digest[2] += c;
  digest[3] += d;
  digest[4] += e;

  /* Count the number of transformations */
  transforms++;
}

static void buffer_to_block(const unsigned char *buffer, uint32_t block[DIGEST_BLOCK_INTS]) {
  /* Convert the std::string (byte buffer) to a uint32_t array (MSB) */
  for (unsigned int i = 0; i < DIGEST_BLOCK_INTS; i++) {
    block[i] = (buffer[4 * i + 3] & 0xff)
        | (buffer[4 * i + 2] & 0xff) << 8
        | (buffer[4 * i + 1] & 0xff) << 16
        | (buffer[4 * i + 0] & 0xff) << 24;
  }
}

static int copyMemory(void *dest, int length, const void *src, int src_pos, int src_size) {
  int src_remaining = src_size - src_pos;
  const unsigned char *psrc = ((const unsigned char *) src) + src_pos;
  unsigned char *pdest = (unsigned char *) dest;
  int outlen = 0;
  while (length-- && src_remaining--) {
    *(pdest++) = *(psrc++);
    outlen++;
  }
  return outlen;
}

Sha1Digest::Sha1Digest() {
  reset();
}

int Sha1Digest::getDigestSize() {
  return 20;
}

void Sha1Digest::reset() {
  int i;

  /* SHA1 initialization constants */
  state_[0] = 0x67452301;
  state_[1] = 0xefcdab89;
  state_[2] = 0x98badcfe;
  state_[3] = 0x10325476;
  state_[4] = 0xc3d2e1f0;

  transforms_ = 0;
  buffer_pos_ = 0;

  for (i = 0; i < DIGEST_BLOCK_BYTES; i++) {
    buffer_[i] = 0;
  }
}

void Sha1Digest::update(const void *data, int length) {
  int pos = 0;

  while (true) {
    int read_len;
    read_len = copyMemory(&buffer_[buffer_pos_], DIGEST_BLOCK_BYTES - buffer_pos_, data, pos, length);
    buffer_pos_ += read_len;
    if (buffer_pos_ != DIGEST_BLOCK_BYTES) {
      return;
    }

    uint32_t block[DIGEST_BLOCK_INTS];
    buffer_to_block(buffer_, block);
    transform(state_, block, transforms_);
    buffer_pos_ = 0;

    pos += DIGEST_BLOCK_BYTES;
  }
}

void Sha1Digest::final(void *data) {
  uint64_t total_bits = (transforms_ * DIGEST_BLOCK_BYTES + buffer_pos_) * 8;

  /* Padding */
  buffer_[buffer_pos_++] = 0x80u;
  size_t orig_size = buffer_pos_;
  while (buffer_pos_ < DIGEST_BLOCK_BYTES) {
    buffer_[buffer_pos_++] = 0x00u;
  }

  uint32_t block[DIGEST_BLOCK_INTS];
  buffer_to_block(buffer_, block);

  if (orig_size > (DIGEST_BLOCK_BYTES - 8)) {
    transform(state_, block, transforms_);
    for (int i = 0; i < DIGEST_BLOCK_INTS - 2; i++) {
      block[i] = 0;
    }
  }

  /* Append total_bits, split this uint64_t into two uint32_t */
  block[DIGEST_BLOCK_INTS - 1] = (uint32_t) total_bits;
  block[DIGEST_BLOCK_INTS - 2] = (uint32_t) (total_bits >> 32);
  transform(state_, block, transforms_);

  int k = 0;
  unsigned char *dest = (unsigned char *) data;
  for (int i = 0; i < (sizeof(state_) / sizeof(state_[0])); i++) {
    uint32_t cur = state_[i];
    for (int j = 3; j >= 0; j--) {
      dest[k++] = (unsigned char) (cur >> (j * 8));
    }
  }

  reset();
}

} // namespace random
} // namespace jcu