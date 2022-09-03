#pragma once

struct RGBA {
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
  bool operator!=(const RGBA& rhs) const noexcept { return !(*this == rhs); }
  bool operator<(const RGBA& rhs) const noexcept {
    if (r != rhs.r) {
      return r < rhs.r;
    }
    if (g != rhs.g) {
      return g < rhs.g;
    }
    if (b != rhs.b) {
      return b < rhs.b;
    }
    return a < rhs.a;
  }
  int operator[](std::size_t channel) const noexcept {
    switch (channel) {
      case 0: return r;
      case 1: return g;
      case 2: return b;
      default: return a;
    }
  }
  int& operator[](std::size_t channel) noexcept {
    switch (channel) {
      case 0: return r;
      case 1: return g;
      case 2: return b;
      default: return a;
    }
  }
};
