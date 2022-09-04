#pragma once

#include <cstdint>

struct RGBA {
  union {
    uint8_t data[4];
    uint32_t data32;
  };
  RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0);
  bool operator<(const RGBA& rhs) const noexcept {
    return data32 < rhs.data32;
  }
  bool operator==(const RGBA& rhs) const noexcept {
    return data32 == rhs.data32;
  }
  bool operator!=(const RGBA& rhs) const noexcept { return !(*this == rhs); }
  uint8_t operator[](std::size_t channel) const noexcept {
    return data[channel];
  }
  uint8_t& operator[](std::size_t channel) noexcept {
    return data[channel];
  }
  friend std::ostream& operator<<(std::ostream& os, const RGBA& rgba) {
    os << "RGBA(" << int(rgba[0]) << ", " << int(rgba[1])
       << ", " << int(rgba[2]) << ", " << int(rgba[3]) << ")";
    return os;
  }
};

namespace std {
  inline std::string to_string(const RGBA& x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
  }
}
