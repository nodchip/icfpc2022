#pragma once

#include <cstdint>

struct RGBA {
  int data[4];
  RGBA(int r = 0, int g = 0, int b = 0, int a = 0);
  bool operator<(const RGBA& rhs) const noexcept {
    return std::memcmp(data, rhs.data, sizeof(data)) < 0;
  }
  bool operator==(const RGBA& rhs) const noexcept {
    return std::memcmp(data, rhs.data, sizeof(data)) == 0;
  }
  bool operator!=(const RGBA& rhs) const noexcept { return !(*this == rhs); }
  int operator[](std::size_t channel) const noexcept {
    return data[channel];
  }
  int& operator[](std::size_t channel) noexcept {
    return data[channel];
  }
  friend std::ostream& operator<<(std::ostream& os, const RGBA& rgba) {
    os << "RGBA(" << rgba[0] << ", " << rgba[1] << ", " << rgba[2] << ", " << rgba[3] << ")";
    return os;
  }
};
