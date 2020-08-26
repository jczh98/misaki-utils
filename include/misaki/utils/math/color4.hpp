#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TColor4 {
 public:
  using Self = TColor4<Value>;
  Value r, g, b, a;

  MSK_CPU_GPU constexpr TColor4() noexcept : r(0), g(0), b(0), a(0) {}

  MSK_CPU_GPU constexpr TColor4(Value r, Value g, Value b, Value a) noexcept : r(r), g(g), b(b), a(a) {}

  MSK_CPU_GPU constexpr TColor4(Value r) noexcept : r(r), g(r), b(r), a(r) {}

  MSK_CPU_GPU constexpr TColor4(const TColor3<Value> &col) noexcept : r(col.r), g(col.g), b(col.b), a(Value(1)) {}

  MSK_CPU_GPU constexpr TColor4(const TVector4<Value> &vec) noexcept : r(vec.x), g(vec.y), b(vec.z), a(vec.w) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  MSK_CPU_GPU constexpr TColor4(const TColor4<T> &col) noexcept : r(Value(col.r)), g(Value(col.g)), b(Value(col.b)), a(Value(col.a)) {}

  // Component access operation
  MSK_CPU_GPU Value &operator[](size_t idx) noexcept {
    return *(&r + idx);
  }

  MSK_CPU_GPU const Value &operator[](size_t idx) const noexcept {
    return *(&r + idx);
  }

  // Inline math functions
  MSK_CPU_GPU Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(r, min_v, max_v),
                std::clamp(g, min_v, max_v),
                std::clamp(b, min_v, max_v),
                std::clamp(a, min_v, max_v));
  }

  MSK_CPU_GPU auto hsum() const noexcept {
    return r + g + b + a;
  }

  MSK_CPU_GPU auto hprod() const noexcept {
    return r * g * b * a;
  }

  MSK_CPU_GPU bool is_valid() const {
    auto valid = [&](Value x) -> bool { return x >= 0 && std::isfinite(x); };
    return valid(r) && valid(g) && valid(b) && valid(a);
  }

  MSK_CPU_GPU TColor3<Value> divide_by_alpha() const {
    if (a != 0) {
      return TColor3<Value>(r / a, g / a, b / a);
    } else {
      return TColor3<Value>();
    }
  }

  // Unary squared_norm
  MSK_CPU_GPU Self &
  operator+=(const Self &rhs) noexcept {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    a += rhs.a return *this;
  }

  MSK_CPU_GPU Self &operator-=(const Self &rhs) noexcept {
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;
    a -= rhs.a;
    return *this;
  }

  MSK_CPU_GPU Self &operator*=(const Self &rhs) noexcept {
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    a *= rhs.a;
    return *this;
  }

  MSK_CPU_GPU Self &operator/=(const Self &rhs) noexcept {
    r /= rhs.r;
    g /= rhs.g;
    b /= rhs.b;
    a /= rhs.a;
    return *this;
  }

  MSK_CPU_GPU Self &operator+=(Value rhs) noexcept {
    r += rhs;
    g += rhs;
    b += rhs;
    a += rhs;
    return *this;
  }

  MSK_CPU_GPU Self &operator-=(Value rhs) noexcept {
    r -= rhs;
    g -= rhs;
    b -= rhs;
    a -= rhs;
    return *this;
  }

  MSK_CPU_GPU Self &operator*=(Value rhs) noexcept {
    r *= rhs;
    g *= rhs;
    b *= rhs;
    a *= rhs;
    return *this;
  }

  MSK_CPU_GPU Self &operator/=(Value rhs) noexcept {
    r /= rhs;
    g /= rhs;
    b /= rhs;
    a /= rhs;
    return *this;
  }
};

// Unary operation
template <typename Value>
MSK_CPU_GPU TColor4<Value> operator-(const TColor4<Value> &col) noexcept {
  return TColor4<Value>(-col.r, -col.g, -col.b, -col.a);
}

// Binary operation
template <typename Value>
MSK_CPU_GPU TColor4<Value> operator+(const TColor4<Value> &lhs, const TColor4<Value> &rhs) noexcept {
  return TColor4<Value>(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
}

template <typename Value>
MSK_CPU_GPU TColor4<Value> operator-(const TColor4<Value> &lhs, const TColor4<Value> &rhs) noexcept {
  return TColor4<Value>(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
}

template <typename Value>
MSK_CPU_GPU TColor4<Value> operator*(const TColor4<Value> &lhs, const TColor4<Value> &rhs) noexcept {
  return TColor4<Value>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
}

template <typename Value>
MSK_CPU_GPU TColor4<Value> operator/(const TColor4<Value> &lhs, const TColor4<Value> &rhs) noexcept {
  return TColor4<Value>(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TColor4<Value> operator+(const TColor4<Value> &lhs, T rhs) noexcept {
  return TColor4<Value>(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TColor4<Value> operator-(const TColor4<Value> &lhs, T rhs) noexcept {
  return TColor4<Value>(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TColor4<Value> operator*(const TColor4<Value> &lhs, T rhs) noexcept {
  return TColor4<Value>(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TColor4<Value> operator/(const TColor4<Value> &lhs, T rhs) noexcept {
  return TColor4<Value>(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
}

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TColor4<Value> &col) {
  oss << "[";
  oss << col.r << ", ";
  oss << col.g << ", ";
  oss << col.b << ", ";
  oss << col.a;
  oss << "]";
  return oss;
}

// Type alias
using Color4f = TColor4<float>;
using Color4d = TColor4<double>;
using Color4b = TColor4<unsigned char>;

}  // namespace misaki::math