/**
 * @file	win-random-provider.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/random-provider.h"

#include <tchar.h>
#include <windows.h>
#include <wincrypt.h>
#include <bcrypt.h>

typedef NTSTATUS (WINAPI *fnBCryptGenRandom_t)(
    _In_opt_                        BCRYPT_ALG_HANDLE   hAlgorithm,
    _Out_writes_bytes_(cbBuffer)    PUCHAR  pbBuffer,
    _In_                            ULONG   cbBuffer,
    _In_                            ULONG   dwFlags
    );

namespace jcu {
namespace random {

namespace platform_windows {

class WinCryptRandomProvider : public RandomProvider {
 private:
  HMODULE bcrypt_module_;
  fnBCryptGenRandom_t fnBCryptGenRandom_;

  HCRYPTPROV crypt_prov_handler_;
  DWORD error_code_;

 public:
  WinCryptRandomProvider()
      : crypt_prov_handler_(0), error_code_(0), fnBCryptGenRandom_(nullptr)
  {
    BOOL rv;
    HCRYPTPROV cryptprovider = 0;
    DWORD error_code = 0;

    bcrypt_module_ = ::LoadLibrary(_T("bcrypt.dll"));
    if (bcrypt_module_) {
      fnBCryptGenRandom_ = (fnBCryptGenRandom_t) ::GetProcAddress(bcrypt_module_, "BCryptGenRandom");
    } else {
      error_code_ = ::GetLastError();

      ::SetLastError(0);
      rv = ::CryptAcquireContext(
          &cryptprovider, NULL,
          MS_ENHANCED_PROV, PROV_RSA_FULL,
          CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
      error_code = ::GetLastError();

      if (error_code == NTE_BAD_KEYSET) {
        rv = ::CryptAcquireContext(
            &cryptprovider, NULL,
            MS_ENHANCED_PROV, PROV_RSA_FULL,
            CRYPT_NEWKEYSET | CRYPT_SILENT);
        error_code = ::GetLastError();
      }

      if (!rv) {
        /* try the default provider */
        rv = ::CryptAcquireContext(
            &cryptprovider, NULL,
            NULL, PROV_RSA_FULL,
            CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
        error_code = ::GetLastError();

        if (error_code == NTE_BAD_KEYSET) {
          rv = ::CryptAcquireContext(
              &cryptprovider, NULL,
              MS_ENHANCED_PROV, PROV_RSA_FULL,
              CRYPT_NEWKEYSET | CRYPT_SILENT);
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
        error_code_ = 0;
      } else {
        error_code_ = ::GetLastError();
      }
    }
  }

  ~WinCryptRandomProvider() override {
    if (bcrypt_module_) {
      ::FreeLibrary(bcrypt_module_);
    }
    if (crypt_prov_handler_) {
      ::CryptReleaseContext(crypt_prov_handler_, 0);
      crypt_prov_handler_ = NULL;
    }
  }

  int nextBytes(unsigned char *buffer, int length) override {
    if (bcrypt_module_) {
      NTSTATUS status = fnBCryptGenRandom_(NULL, (BYTE*)buffer, length, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
      if (status == 0) {
        return length;
      }
    }
    if (crypt_prov_handler_) {
      if (::CryptGenRandom(crypt_prov_handler_, length, (BYTE *) buffer)) {
        return length;
      }
    }
    return -1;
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
