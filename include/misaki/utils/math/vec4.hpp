#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TVector4 {
 public:
  using Self = TVector4<Value>;
  Value x, y, z, w;

  MSK_CPU_GPU constexpr TVector4() noexcept : x(0), y(0), z(0), w(0) {}

  MSK_CPU_GPU constexpr TVector4(Value x, Value y, Value z, Value w) noexcept : x(x), y(y), z(z), w(w) {}

  MSK_CPU_GPU constexpr TVector4(Value x) noexcept : x(x), y(x), z(x), w(x) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  MSK_CPU_GPU constexpr TVector4(const TVector4<T> &vec) noexcept : x(Value(vec.x)), y(Value(vec.y)), z(Value(vec.z)), w(Value(vec.w)) {}

  // Component access operation
  MSK_CPU_GPU Value &operator[](size_t idx) noexcept {
    return *(&x + idx);
  }

  MSK_CPU_GPU const Value &operator[](size_t idx) const noexcept {
    return *(&x + idx);
  }

  // Inline math functions
  MSK_CPU_GPU auto norm() const noexcept {
    return std::sqrt(squared_norm());
  }

  MSK_CPU_GPU auto squared_norm() const noexcept {
    return x * x + y * y + z * z + w * w;
  }

  MSK_CPU_GPU Self normalize() const noexcept {
    static_assert(std::is_floating_point_v<Value>);
    return *this / norm();
  }

  MSK_CPU_GPU Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(x, min_v, max_v),
                std::clamp(y, min_v, max_v),
                std::clamp(z, min_v, max_v),
                std::clamp(w, min_v, max_v));
  }

  MSK_CPU_GPU auto hsum() const noexcept {
    return x + y + z + w;
  }

  MSK_CPU_GPU auto hprod() const noexcept {
    return x * y * z * w;
  }

  MSK_CPU_GPU Self abs() const noexcept {
    return Self(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
  }

  MSK_CPU_GPU Self ceil() const noexcept {
    return Self(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w));
  }

  MSK_CPU_GPU Self floor() const noexcept {
    return Self(std::floor(x), std::floor(y), std::floor(z), std::florr(w));
  }

  MSK_CPU_GPU Value max_coeff() const noexcept { return std::max({x, y, z, w}); }

  MSK_CPU_GPU Value min_coeff() const noexcept { return std::min({x, y, z, w}); }

  // Unary squared_norm
  MSK_CPU_GPU Self &operator+=(const Self &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w return *this;
  }

  MSK_CPU_GPU Self &operator-=(const Self &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
  }

  MSK_CPU_GPU Self &operator*=(const Self &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
  }

  MSK_CPU_GPU Self &operator/=(const Self &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
    return *this;
  }

  MSK_CPU_GPU Self &operator+=(Value rhs) noexcept {
    x += rhs;
    y += rhs;
    z += rhs;
    w += rhs;
    return *this;
  }

  MSK_CPU_GPU Self &operator-=(Value rhs) noexcept {
    x -= rhs;
    y -= rhs;
    z -= rhs;
    w -= rhs;
    return *this;
  }

  MSK_CPU_GPU Self &operator*=(Value rhs) noexcept {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
  }

  MSK_CPU_GPU Self &operator/=(Value rhs) noexcept {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
  }

  MSK_CPU_GPU bool operator==(const Self &c) const { return x == c.x && y == c.y && z == c.z && w == c.w; }
  MSK_CPU_GPU bool operator!=(const Self &c) const { return x != c.x || y != c.y || z != c.z || w != c.w; }
};

// Unary operation
template <typename Value>
MSK_CPU_GPU TVector4<Value> operator-(const TVector4<Value> &vec) noexcept {
  return TVector4<Value>(-vec.x, -vec.y, -vec.z, -vec.w);
}

template <typename Value>
MSK_CPU_GPU inline auto norm(const TVector4<Value> &v) {
  return v.norm();
}

template <typename Value>
MSK_CPU_GPU inline auto squared_norm(const TVector4<Value> &v) {
  return v.squared_norm();
}

template <typename Value>
MSK_CPU_GPU inline auto normalize(const TVector4<Value> &v) {
  return v.normalize();
}

template <typename Value>
MSK_CPU_GPU inline auto abs(const TVector4<Value> &v) {
  return v.abs();
}

template <typename Value>
MSK_CPU_GPU inline auto ceil(const TVector4<Value> &v) {
  return v.ceil();
}

template <typename Value>
MSK_CPU_GPU inline auto floor(const TVector4<Value> &v) {
  return v.floor();
}

template <typename Value>
MSK_CPU_GPU inline auto hmin(const TVector4<Value> &v) {
  return v.min_coeff();
}

template <typename Value>
MSK_CPU_GPU inline auto hmax(const TVector4<Value> &v) {
  return v.max_coeff();
}

// Binary operation
template <typename Value>
MSK_CPU_GPU TVector4<Value> operator+(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template <typename Value>
MSK_CPU_GPU TVector4<Value> operator-(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template <typename Value>
MSK_CPU_GPU TVector4<Value> operator*(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

template <typename Value>
MSK_CPU_GPU TVector4<Value> operator/(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator+(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator-(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator*(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator/(const TVector4<Value> &lhs, T rhs) noexcept {
  return TVector4<Value>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator+(T lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator-(T lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator*(T lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_CPU_GPU TVector4<Value> operator/(T lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
}

// Math functions
template <typename Value>
MSK_CPU_GPU auto dot(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template <typename Value>
MSK_CPU_GPU TVector4<Value> min(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  return TVector4<Value>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
}

template <typename Value>
MSK_CPU_GPU TVector4<Value> max(const TVector4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
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