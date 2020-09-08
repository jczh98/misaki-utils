#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TVector3 {
 public:
  using Self = TVector3<Value>;
  Value x, y, z;

  MSK_XPU constexpr TVector3() noexcept : x(0), y(0), z(0) {}

  MSK_XPU constexpr TVector3(Value x, Value y, Value z) noexcept : x(x), y(y), z(z) {}

  MSK_XPU constexpr TVector3(Value x) noexcept : x(x), y(x), z(x) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  MSK_XPU constexpr TVector3(const TVector3<T> &vec) noexcept : x(Value(vec.x)), y(Value(vec.y)), z(Value(vec.z)) {}

  // Component access operation
  MSK_XPU Value &operator[](size_t idx) noexcept {
    return *(&x + idx);
  }

  MSK_XPU const Value &operator[](size_t idx) const noexcept {
    return *(&x + idx);
  }

  // Inline math functions
  MSK_XPU auto norm() const noexcept {
    return std::sqrt(squared_norm());
  }

  MSK_XPU auto squared_norm() const noexcept {
    return x * x + y * y + z * z;
  }

  MSK_XPU Self normalize() const noexcept {
    static_assert(std::is_floating_point_v<Value>);
    return *this / norm();
  }

  MSK_XPU Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(x, min_v, max_v),
                std::clamp(y, min_v, max_v),
                std::clamp(z, min_v, max_v));
  }

  MSK_XPU Self abs() const noexcept {
    return Self(std::abs(x), std::abs(y), std::abs(z));
  }

  MSK_XPU Self ceil() const noexcept {
    return Self(std::ceil(x), std::ceil(y), std::ceil(z));
  }

  MSK_XPU Self floor() const noexcept {
    return Self(std::floor(x), std::floor(y), std::floor(z));
  }

  MSK_XPU auto hsum() const noexcept {
    return x + y + z;
  }

  MSK_XPU auto hprod() const noexcept {
    return x * y * z;
  }

  MSK_XPU Value max_coeff() const noexcept { return std::max({x, y, z}); }

  MSK_XPU Value min_coeff() const noexcept { return std::min({x, y, z}); }

  // Unary squared_norm
  MSK_XPU Self &operator+=(const Self &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  MSK_XPU Self &operator-=(const Self &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  MSK_XPU Self &operator*=(const Self &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }

  MSK_XPU Self &operator/=(const Self &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  }

  MSK_XPU Self &operator+=(Value rhs) noexcept {
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
  }

  MSK_XPU Self &operator-=(Value rhs) noexcept {
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
  }

  MSK_XPU Self &operator*=(Value rhs) noexcept {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  MSK_XPU Self &operator/=(Value rhs) noexcept {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }

  MSK_XPU bool operator==(const Self &c) const { return x == c.x && y == c.y && z == c.z; }
  MSK_XPU bool operator!=(const Self &c) const { return x != c.x || y != c.y || z != c.z; }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};
// Unary operation
template <typename Value>
MSK_XPU TVector3<Value> operator-(const TVector3<Value> &vec) noexcept {
  return TVector3<Value>(-vec.x, -vec.y, -vec.z);
}

template <typename Value>
MSK_XPU inline auto norm(const TVector3<Value> &v) {
  return v.norm();
}

template <typename Value>
MSK_XPU inline auto squared_norm(const TVector3<Value> &v) {
  return v.squared_norm();
}

template <typename Value>
MSK_XPU inline auto normalize(const TVector3<Value> &v) {
  return v.normalize();
}

template <typename Value>
MSK_XPU inline auto abs(const TVector3<Value> &v) {
  return v.abs();
}

template <typename Value>
MSK_XPU inline auto ceil(const TVector3<Value> &v) {
  return v.ceil();
}

template <typename Value>
MSK_XPU inline auto floor(const TVector3<Value> &v) {
  return v.floor();
}

template <typename Value>
MSK_XPU inline auto hmin(const TVector3<Value> &v) {
  return v.min_coeff();
}

template <typename Value>
MSK_XPU inline auto hmax(const TVector3<Value> &v) {
  return v.max_coeff();
}

// Binary operation
template <typename Value>
MSK_XPU TVector3<Value> operator+(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename Value>
MSK_XPU TVector3<Value> operator-(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <typename Value>
MSK_XPU TVector3<Value> operator*(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template <typename Value>
MSK_XPU TVector3<Value> operator/(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator+(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator-(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator*(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator/(const TVector3<Value> &lhs, T rhs) noexcept {
  return TVector3<Value>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator+(T lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator-(T lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator*(T lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_XPU TVector3<Value> operator/(T lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}
// Math functions
template <typename Value>
MSK_XPU auto dot(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename Value>
MSK_XPU auto cross(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(
      lhs.y * rhs.z - lhs.z * rhs.y,
      lhs.z * rhs.x - lhs.x * rhs.z,
      lhs.x * rhs.y - lhs.y * rhs.x);
}

template <typename Value>
MSK_XPU TVector3<Value> min(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
}

template <typename Value>
MSK_XPU TVector3<Value> max(const TVector3<Value> &lhs, const TVector3<Value> &rhs) noexcept {
  return TVector3<Value>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
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

// Compute orthonormal basis
// Based on paper "Building an Orthonormal Basis, Revisited"
template <typename Float>
MSK_XPU void coordinate_system(const TVector3<Float> &n, TVector3<Float> *v1, TVector3<Float> *v2) {
  Float sign = std::copysign(1.f, n.z);
  const Float a = -1.f / (sign + n.z);
  const Float b = n.x * n.y * a;
  *v1 = {1.f + sign * n.x * n.x * a, sign * b, -sign * n.x};
  *v2 = {b, sign + n.y * n.y * a, -n.y};
}

// Type alias
using Vector3f = TVector3<float>;
using Vector3d = TVector3<double>;
using Vector3i = TVector3<int>;
using Vector3u = TVector3<uint32_t>;

}  // namespace misaki::math