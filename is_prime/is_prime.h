#pragma once

#include <cstdint>


inline bool IsPrime(uint64_t x) {
  if (x == 1) {
    return false;
  }
  if (x == 2) {
    return true;
  }
  if (x % 2 == 0) {
    return false;
  }
  return true;
}

