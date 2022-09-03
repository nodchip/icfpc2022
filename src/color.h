#pragma once

struct RGBA
{
  int r;
  int g;
  int b;
  int a;
  RGBA(int r = 0, int g = 0, int b = 0, int a = 0);
  bool operator<(const RGBA& rhs) const noexcept {
    if (r != rhs.r) return r < rhs.r;
    if (g != rhs.g) return g < rhs.g;
    if (b != rhs.b) return b < rhs.b;
    return a < rhs.a;
  }
  bool operator==(const RGBA& rhs) const noexcept {
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
  }
  bool operator!=(const RGBA& rhs) const noexcept {
    return !(*this == rhs);
  }
};
