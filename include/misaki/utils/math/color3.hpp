#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TColor3 {
 public:
  using Self = TColor3<Value>;
  Value r, g, b;

  MSK_XPU constexpr TColor3() noexcept : r(0), g(0), b(0) {}

  MSK_XPU constexpr TColor3(Value r, Value g, Value b) noexcept : r(r), g(g), b(b) {}

  MSK_XPU constexpr TColor3(Value r) noexcept : r(r), g(r), b(r) {}

  MSK_XPU constexpr TColor3(const TVector3<Value> &vec) noexcept : r(vec.x), g(vec.y), b(vec.z) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  MSK_XPU constexpr TColor3(const TColor3<T> &col) noexcept : r(Value(col.r)), g(Value(col.g)), b(Value(col.b)) {}

  // Component access operation
  MSK_XPU Value &operator[](size_t idx) noexcept {
    return *(&r + idx);
  }

  MSK_XPU const Value &operator[](size_t idx) const noexcept {
    return *(&r + idx);
  }

  // Inline math functions
  MSK_XPU Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(r, min_v, max_v),
                std::clamp(g, min_v, max_v),
                std::clamp(b, min_v, max_v));
  }

  MSK_XPU auto hsum() const noexcept {
    return r + g + b;
  }

  MSK_XPU auto hprod() const noexcept {
    return r * g * b;
  }

  MSK_XPU auto luminance() const {
    return r * 0.212671f + g * 0.715160f + b * 0.072169f;
  }

  MSK_XPU Self to_srgb() const {
    Self result;
    for (int i = 0; i < 3; ++i) {
      Value value = this->operator[](i);
      if (value <= 0.0031308f)
        result[i] = 12.92f * value;
      else
        result[i] = (1.0f + 0.055f) * std::pow(value, 1.0f / 2.4f) - 0.055f;
    }
    return result;
  }

  MSK_XPU Self to_linear() const {
    Self result;
    for (int i = 0; i < 3; ++i) {
      Value value = this->operator[](i);
      if (value <= 0.04045f)
        result[i] = value * (1.0f / 12.92f);
      else
        result[i] = std::pow((value + 0.055f) * (1.0f / 1.055f), 2.4f);
    }
    return result;
  }

  MSK_XPU bool is_black() const {
    return r == 0 || g == 0 || b == 0;
  }

  MSK_XPU bool is_valid() const {
    auto valid = [&](Value x) -> bool { return x >= 0 && std::isfinite(x); };
    return valid(r) && valid(g) && valid(b);
  }

  // Unary squared_norm
  MSK_XPU Self &operator+=(const Self &rhs) noexcept {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
  }

  MSK_XPU Self &operator-=(const Self &rhs) noexcept {
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;
    return *this;
  }

  MSK_XPU Self &operator*=(const Self &rhs) noexcept {
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    return *this;
  }

  MSK_XPU Self &operator/=(const Self &rhs) noexcept {
    r /= rhs.r;
    g /= rhs.g;
    b /= rhs.b;
    return *this;
  }

  MSK_XPU Self &operator+=(Value rhs) noexcept {
    r += rhs;
    g += rhs;
    b += rhs;
    return *this;
  }

  MSK_XPU Self &operator-=(Value rhs) noexcept {
    r -= rhs;
    g -= rhs;
    b -= rhs;
    return *this;
  }

  MSK_XPU Self &operator*=(Value rhs) noexcept {
    r *= rhs;
    g *= rhs;
    b *= rhs;
    return *this;
  }

  MSK_XPU Self &operator/=(Value rhs) noexcept {
    r /= rhs;
    g /= rhs;
    b /= rhs;
    return *this;
  }
};

// Unary operation
template <typename Value>
MSK_XPU TColor3<Value> operator-(const TColor3<Value> &col) noexcept {
  return TColor3<Value>(-col.r, -col.g, -col.b);
}

// Binary operation
template <typename Value>
MSK_XPU TColor3<Value> operator+(const TColor3<Value> &lhs, const TColor3<Value> &rhs) noexcept {
  return TColor3<Value>(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}

template <typename Value>
MSK_XPU TColor3<Value> operator-(const TColor3<Value> &lhs, const TColor3<Value> &rhs) noexcept {
  return TColor3<Value>(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
}

template <typename Value>
MSK_XPU TColor3<Value> operator*(const TColor3<Value> &lhs, const TColor3<Value> &rhs) noexcept {
  return TColor3<Value>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

template <typename Value>
MSK_XPU TColor3<Value> operator/(const TColor3<Value> &lhs, const TColor3<Value> &rhs) noexcept {
  return TColor3<Value>(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TColor3<Value> operator+(const TColor3<Value> &lhs, T rhs) noexcept {
  return TColor3<Value>(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TColor3<Value> operator-(const TColor3<Value> &lhs, T rhs) noexcept {
  return TColor3<Value>(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TColor3<Value> operator*(const TColor3<Value> &lhs, T rhs) noexcept {
  return TColor3<Value>(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TColor3<Value> operator/(const TColor3<Value> &lhs, T rhs) noexcept {
  return TColor3<Value>(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs);
}

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TColor3<Value> &col) {
  oss << "[";
  oss << col.r << ", ";
  oss << col.g << ", ";
  oss << col.b;
  oss << "]";
  return oss;
}

// Type alias
using Color3f = TColor3<float>;
using Color3d = TColor3<double>;
using Color3b = TColor3<unsigned char>;

}  // namespace misaki::math