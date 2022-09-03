#include "stdafx.h"
#include "xorshift.h"

namespace {
  static uint64_t x = 88172645463325252ULL;
}

uint64_t xor64(void) {
  x = x ^ (x << 7);
  return x = x ^ (x >> 9);
}
