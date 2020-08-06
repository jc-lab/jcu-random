/**
 * @file	urandom-random-provider.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jc-lab. All rights reserved.
 */

#include "jcu-random/random-provider.h"

#include <unistd.h>
#include <fcntl.h>

namespace jcu {
namespace random {

namespace platform_linux {

class UrandomProvider : public RandomProvider {
 private:
  int fd_;

 public:
  UrandomProvider() {
    fd_ = open("/dev/urandom", O_RDONLY);
  }

  ~UrandomProvider() override {
    if (fd_ != -1) {
      close(fd_);
      fd_ = -1;
    }
  }

  int nextBytes(unsigned char *buffer, int length) override {
    return (int) read(fd_, buffer, length);
  }
};

}

std::unique_ptr<RandomProvider> getSystemRandomProvider() {
  return std::unique_ptr<platform_linux::UrandomProvider>(new platform_linux::UrandomProvider());
}

} // namespace random
} // namespace jcu
