/**
 * @file	win-random-provider.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/random-provider.h"

#include <windows.h>
#include <wincrypt.h>

namespace jcu {
namespace random {

namespace platform_windows {

class WinCryptRandomProvider : public RandomProvider {
 private:
  HCRYPTPROV crypt_prov_handler_;
  DWORD error_code_;

 public:
  WinCryptRandomProvider()
      : crypt_prov_handler_(0), error_code_(0) {
    BOOL rv;
    HCRYPTPROV cryptprovider = 0;

    ::SetLastError(0);
    rv = ::CryptAcquireContext(
        &cryptprovider, NULL,
        MS_ENHANCED_PROV, PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT);

    if (::GetLastError() == NTE_BAD_KEYSET) {
      rv = ::CryptAcquireContext(
          &cryptprovider, NULL,
          MS_ENHANCED_PROV, PROV_RSA_FULL,
          CRYPT_NEWKEYSET);
    }

    if (rv) {
      /* try the default provider */
      rv = ::CryptAcquireContext(
          &cryptprovider, NULL, 0, PROV_RSA_FULL,
          CRYPT_VERIFYCONTEXT);

      if (::GetLastError() == NTE_BAD_KEYSET) {
        rv = ::CryptAcquireContext(
            &cryptprovider, NULL,
            MS_ENHANCED_PROV, PROV_RSA_FULL,
            CRYPT_NEWKEYSET);
      }
    }

    if (rv) {
      /* try just a default random number generator */
      rv = ::CryptAcquireContext(
          &cryptprovider, NULL, 0, PROV_RNG,
          CRYPT_VERIFYCONTEXT);
    }

    if (cryptprovider) {
      crypt_prov_handler_ = cryptprovider;
    } else {
      error_code_ = ::GetLastError();
    }
  }

  ~WinCryptRandomProvider() override {
    if (crypt_prov_handler_) {
      ::CryptReleaseContext(crypt_prov_handler_, 0);
      crypt_prov_handler_ = NULL;
    }
  }

  int nextBytes(unsigned char *buffer, int length) override {
    ::CryptGenRandom(crypt_prov_handler_, length, (BYTE *) buffer);
    return length;
  }

  DWORD getErrorCode() const {
    return error_code_;
  }
};

}

std::unique_ptr<RandomProvider> getSystemRandomProvider() {
  return std::unique_ptr<platform_windows::WinCryptRandomProvider>(new platform_windows::WinCryptRandomProvider());
}

} // namespace random
} // namespace jcu
