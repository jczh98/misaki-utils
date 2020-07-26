#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TVector3 {
 public:
  using Self = TVector3<Value>;
  Value x, y, z;

  constexpr TVector3() noexcept : x(0), y(0), z(0) {}

  constexpr TVector3(Value x, Value y, Value z) noexcept : x(x), y(y), z(z) {}

  constexpr TVector3(Value x) noexcept : x(x), y(x), z(x) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  constexpr TVector3(const TVector3<T> &vec) noexcept : x(Value(vec.x)), y(Value(vec.y)), z(Value(vec.z)) {}

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
    return x * x + y * y + z * z;
  }

  Self normalize() const noexcept {
    static_assert(std::is_floating_point_v<Value>);
    return *this / norm();
  }

  Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(x, min_v, max_v),
                std::clamp(y, min_v, max_v),
                std::clamp(z, min_v, max_v));
  }

  auto hsum() const noexcept {
    return x + y + z;
  }

  auto hprod() const noexcept {
    return x * y * z;
  }

  // Unary squared_norm
  Self &operator+=(const Self &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  Self &operator-=(const Self &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  Self &operator*=(const Self &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }

  Self &operator/=(const Self &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  }

  Self &operator+=(Value rhs) noexcept {
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
  }

  Self &operator-=(Value rhs) noexcept {
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
  }

  Self &operator*=(Value rhs) noexcept {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  Self &operator/=(Value rhs) noexcept {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }
};

// Unary operation
template <typename Value>
TVector3<Value> operator-(const TVector3<Value> &vec) noexcept {
  return TVector3<Value>(-vec.x, -vec.y, -vec.z);
}

// Binary operation
template <typename Value>
TVector3<Value> operator+(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename Value>
TVector3<Value> operator-(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <typename Value>
TVector3<Value> operator*(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template <typename Value>
TVector3<Value> operator/(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector3<Value> operator+(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector3<Value> operator-(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector3<Value> operator*(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector3<Value> operator/(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

// Math functions
template <typename Value>
auto dot(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename Value>
auto cross(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(
      lhs.y * rhs.z - lhs.z * rhs.y,
      lhs.z * rhs.x - lhs.x * rhs.z,
      lhs.x * rhs.y - lhs.y * rhs.x);
}

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TVector3<Value> &vec) {
  oss << "[";
  oss << vec.x << ", ";
  oss << vec.y << ", ";
  oss << vec.z;
  oss << "]";
  return oss;
}

// Type alias
using Vector3f = TVector3<float>;
using Vector3d = TVector3<double>;
using Vector3i = TVector3<int>;
using Vector3u = TVector3<uint32_t>;

}  // namespace misaki::math