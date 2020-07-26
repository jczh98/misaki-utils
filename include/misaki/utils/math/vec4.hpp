#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TVector4 {
 public:
  using Self = TVector4<Value>;
  Value x, y, z, w;

  constexpr TVector4() noexcept : x(0), y(0), z(0), w(0) {}

  constexpr TVector4(Value x, Value y, Value z, Value w) noexcept : x(x), y(y), z(z), w(w) {}

  constexpr TVector4(Value x) noexcept : x(x), y(x), z(x), w(x) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  constexpr TVector4(const TVector4<T> &vec) noexcept : x(Value(vec.x)), y(Value(vec.y)), z(Value(vec.z)), w(Value(vec.w)) {}

  // Component access operation
  Value &operator[](size_t idx) noexcept {
    return *(&x + idx);
  }

  const Value &operator[](size_t idx) const noexcept {
    return *(&x + idx);
  }

  // Inline math functions
  auto norm() const noexcept {
    return std::sqrt(squared_norm());
  }

  auto squared_norm() const noexcept {
    return x * x + y * y + z * z + w * w;
  }

  Self normalize() const noexcept {
    static_assert(std::is_floating_point_v<Value>);
    return *this / norm();
  }

  Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(x, min_v, max_v),
                std::clamp(y, min_v, max_v),
                std::clamp(z, min_v, max_v),
                std::clamp(w, min_v, max_v));
  }

  auto hsum() const noexcept {
    return x + y + z + w;
  }

  auto hprod() const noexcept {
    return x * y * z * w;
  }

  Value max_coeff() const noexcept { return std::max({x, y, z, w}); }

  Value min_coeff() const noexcept { return std::min({x, y, z, w}); }

  // Unary squared_norm
  Self &operator+=(const Self &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w return *this;
  }

  Self &operator-=(const Self &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
  }

  Self &operator*=(const Self &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
  }

  Self &operator/=(const Self &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
    return *this;
  }

  Self &operator+=(Value rhs) noexcept {
    x += rhs;
    y += rhs;
    z += rhs;
    w += rhs;
    return *this;
  }

  Self &operator-=(Value rhs) noexcept {
    x -= rhs;
    y -= rhs;
    z -= rhs;
    w -= rhs;
    return *this;
  }

  Self &operator*=(Value rhs) noexcept {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
  }

  Self &operator/=(Value rhs) noexcept {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
  }
};

// Unary operation
template <typename Value>
TVector4<Value> operator-(const TVector4<Value> &vec) noexcept {
  return TVector4<Value>(-vec.x, -vec.y, -vec.z, -vec.w);
}

// Binary operation
template <typename Value>
TVector4<Value> operator+(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template <typename Value>
TVector4<Value> operator-(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template <typename Value>
TVector4<Value> operator*(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

template <typename Value>
TVector4<Value> operator/(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector4<Value> operator+(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector4<Value> operator-(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector4<Value> operator*(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector4<Value> operator/(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

// Math functions
template <typename Value>
auto dot(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template <typename Value>
TVector4<Value> cwise_min(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
}

template <typename Value>
TVector4<Value> cwise_max(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w));
}

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TVector4<Value> &vec) {
  oss << "[";
  oss << vec.x << ", ";
  oss << vec.y << ", ";
  oss << vec.z << ", ";
  oss << vec.w;
  oss << "]";
  return oss;
}

// Type alias
using Vector4f = TVector4<float>;
using Vector4d = TVector4<double>;
using Vector4i = TVector4<int>;
using Vector4u = TVector4<uint32_t>;

}  // namespace misaki::math