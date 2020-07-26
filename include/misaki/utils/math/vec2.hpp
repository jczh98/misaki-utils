#pragma once

#include "common.hpp"

namespace misaki::math {

template <typename Value>
class TVector2 {
 public:
  using Self = TVector2<Value>;
  Value x, y;

  constexpr TVector2() noexcept : x(0), y(0) {}

  constexpr TVector2(Value x, Value y) noexcept : x(x), y(y) {}

  constexpr TVector2(Value x) noexcept : x(x), y(x) {}

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
  constexpr TVector2(const TVector2<T> &vec) noexcept : x(Value(vec.x)), y(Value(vec.y)){}

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
    return x * x + y * y;
  }

  Self normalize() const noexcept {
    static_assert(std::is_floating_point_v<Value>);
    return *this / norm();
  }

  Self clamp(Value min_v, Value max_v) const noexcept {
    return Self(std::clamp(x, min_v, max_v),
                std::clamp(y, min_v, max_v));
  }

  auto hsum() const noexcept {
    return x + y;
  }

  auto hprod() const noexcept {
    return x * y;
  }

  // Unary squared_norm
  Self &operator+=(const Self &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Self &operator-=(const Self &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Self &operator*=(const Self &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }

  Self &operator/=(const Self &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  }

  Self &operator+=(Value rhs) noexcept {
    x += rhs;
    y += rhs;
    return *this;
  }

  Self &operator-=(Value rhs) noexcept {
    x -= rhs;
    y -= rhs;
    return *this;
  }

  Self &operator*=(Value rhs) noexcept {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  Self &operator/=(Value rhs) noexcept {
    x /= rhs;
    y /= rhs;
    return *this;
  }
};

// Unary operation
template <typename Value>
TVector2<Value> operator-(const TVector2<Value> &vec) noexcept {
  return TVector2<Value>(-vec.x, -vec.y);
}

// Binary operation
template <typename Value>
TVector2<Value> operator+(const TVector2<Value> &lhs, const TVector2<Value> &rhs) noexcept {
  return TVector2<Value>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <typename Value>
TVector2<Value> operator-(const TVector2<Value> &lhs, const TVector2<Value> &rhs) noexcept {
  return TVector2<Value>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename Value>
TVector2<Value> operator*(const TVector2<Value> &lhs, const TVector2<Value> &rhs) noexcept {
  return TVector2<Value>(lhs.x * rhs.x, lhs.y * rhs.y);
}

template <typename Value>
TVector2<Value> operator/(const TVector2<Value> &lhs, const TVector2<Value> &rhs) noexcept {
  return TVector2<Value>(lhs.x / rhs.x, lhs.y / rhs.y);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector2<Value> operator+(const TVector2<Value> &lhs, T rhs) noexcept {
  return TVector2<Value>(lhs.x + rhs, lhs.y + rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector2<Value> operator-(const TVector2<Value> &lhs, T rhs) noexcept {
  return TVector2<Value>(lhs.x - rhs, lhs.y - rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector2<Value> operator*(const TVector2<Value> &lhs, T rhs) noexcept {
  return TVector2<Value>(lhs.x * rhs, lhs.y * rhs);
}

template <typename Value, typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
TVector2<Value> operator/(const TVector2<Value> &lhs, T rhs) noexcept {
  return TVector2<Value>(lhs.x / rhs, lhs.y / rhs);
}

// Math functions
template <typename Value>
auto dot(const TVector2<Value> &lhs, const TVector2<Value> &rhs) noexcept {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TVector2<Value> &vec) {
  oss << "[";
  oss << vec.x << ", ";
  oss << vec.y;
  oss << "]";
  return oss;
}

// Type alias
using Vector2f = TVector2<float>;
using Vector2d = TVector2<double>;
using Vector2i = TVector2<int>;
using Vector2u = TVector2<uint32_t>;

}  // namespace misaki::math