#pragma once

#include "common.hpp"

namespace misaki {
namespace math {

// Tuple2
template <template <typename> class Child, typename T>
class Tuple2 {
 public:
  Tuple2() = default;

  MSK_CPU_GPU Tuple2(T x, T y) : x(x), y(y) {}

  MSK_CPU_GPU bool has_nan() const { return is_nan(x) || is_nan(y); }

  // Operators
  template <typename U>
  MSK_CPU_GPU auto operator+(const Child<U> &c) const -> Child<decltype(T{} + U{})> {
    return {x + c.x, y + c.y};
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator+=(const Child<U> &c) const {
    x += c.x;
    y += c.y;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator-(const Child<U> &c) const -> Child<decltype(T{} - U{})> {
    return {x - c.x, y - c.y};
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator-=(const Child<U> &c) const {
    x -= c.x;
    y -= c.y;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator*(const Child<U> &c) const -> Child<decltype(T{} * U{})> {
    return {x * c.x, y * c.y};
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator*=(const Child<U> &c) const {
    x *= c.x;
    y *= c.y;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator*(U s) const -> Child<typename std::common_type<T, U>::type> {
    return Child<typename std::common_type<T, U>::type>(s * x, s * y);
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator*=(U s) {
    x *= s;
    y *= s;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator/(U s) const -> Child<typename std::common_type<T, U>::type> {
    return Child<typename std::common_type<T, U>::type>(x / s, y / s);
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator/=(U s) {
    x /= s;
    y /= s;
    return static_cast<Child<T> &>(*this);
  }

  MSK_CPU_GPU Child<T> operator-() const { return {-x, -y}; }

  MSK_CPU_GPU T operator[](int i) const {
    return (i == 0) ? x : y;
  }

  MSK_CPU_GPU T &operator[](int i) {
    return (i == 0) ? x : y;
  }

  MSK_CPU_GPU bool operator==(const Child<T> &c) const { return x == c.x && y == c.y; }
  MSK_CPU_GPU bool operator!=(const Child<T> &c) const { return x != c.x || y != c.y; }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }

  T x{}, y{};
};

// Stream
template <template <class> class C, typename T>
std::ostream &operator<<(
    std::ostream &oss, const Tuple2<C, T> &vec) {
  oss << "[";
  oss << vec.x << ", ";
  oss << vec.y;
  oss << "]";
  return oss;
}

// Inline functions
template <template <class> class C, typename T, typename U>
MSK_CPU_GPU inline auto operator*(U s, const Tuple2<C, T> &t)
    -> C<typename std::common_type<T, U>::type> {
  return t * s;
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> abs(const Tuple2<C, T> &t) {
  return {std::abs(t.x), std::abs(t.y)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> ceil(const Tuple2<C, T> &t) {
  return {std::ceil(t.x), std::ceil(t.y)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> floor(const Tuple2<C, T> &t) {
  return {std::floor(t.x), std::floor(t.y)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline auto lerp(T t, const Tuple2<C, T> &t0, const Tuple2<C, T> &t1) {
  return (1 - t) * t0 + t * t1;
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> min(const Tuple2<C, T> &t0, const Tuple2<C, T> &t1) {
  return {std::min(t0.x, t1.x), std::min(t0.y, t1.y)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline T hmin(const Tuple2<C, T> &t) {
  return std::min({t.x, t.y});
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> max(const Tuple2<C, T> &t0, const Tuple2<C, T> &t1) {
  return {std::max(t0.x, t1.x), std::max(t0.y, t1.y)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline T hmax(const Tuple2<C, T> &t) {
  return std::max({t.x, t.y});
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline T hprod(const Tuple2<C, T> &t) {
  return t.x * t.y;
}

// Vector2 definition
template <typename T>
class Vector2 : public Tuple2<Vector2, T> {
 public:
  // Vector2 Public Methods
  using Tuple2<Vector2, T>::x;
  using Tuple2<Vector2, T>::y;

  Vector2() = default;
  MSK_CPU_GPU Vector2(T x, T y) : Tuple2<Vector2, T>(x, y) {}

  template <typename U>
  MSK_CPU_GPU explicit Vector2(const Point2<U> &p);

  template <typename U>
  MSK_CPU_GPU explicit Vector2(const Vector2<U> &v)
      : Tuple2<Vector2, T>(T(v.x), T(v.y)) {}
};

// Point2 definition
template <typename T>
class Point2 : public Tuple2<Point2, T> {
 public:
  // Point2 Public Methods
  using Tuple2<Point2, T>::x;
  using Tuple2<Point2, T>::y;
  using Tuple2<Point2, T>::has_nan;
  using Tuple2<Point2, T>::operator+;
  using Tuple2<Point2, T>::operator+=;
  using Tuple2<Point2, T>::operator*;
  using Tuple2<Point2, T>::operator*=;

  MSK_CPU_GPU Point2() { x = y = 0; }

  MSK_CPU_GPU Point2(T x, T y) : Tuple2<Point2, T>(x, y) {}

  template <typename U>
  MSK_CPU_GPU explicit Point2(const Point2<U> &v)
      : Tuple2<Point2, T>(T(v.x), T(v.y)) {}

  template <typename U>
  MSK_CPU_GPU explicit Point2(const Vector2<U> &v)
      : Tuple2<Point2, T>(T(v.x), T(v.y)) {}

  template <typename U>
  MSK_CPU_GPU auto operator+(const Vector2<U> &v) const
      -> Point2<decltype(T{} + U{})> {
    return {x + v.x, y + v.y};
  }

  template <typename U>
  MSK_CPU_GPU Point2<T> &operator+=(const Vector2<U> &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  MSK_CPU_GPU Point2<T> operator-() const { return {-x, -y}; }

  template <typename U>
  MSK_CPU_GPU auto operator-(const Point2<U> &p) const
      -> Vector2<decltype(T{} - U{})> {
    return {x - p.x, y - p.y};
  }

  template <typename U>
  MSK_CPU_GPU auto operator-(const Vector2<U> &v) const
      -> Point2<decltype(T{} - U{})> {
    return {x - v.x, y - v.y};
  }

  template <typename U>
  MSK_CPU_GPU Point2<T> &operator-=(const Vector2<U> &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
};

// Vector2 construction
template <typename T>
template <typename U>
Vector2<T>::Vector2(const Point2<U> &p) : Tuple2<Vector2, T>(T(p.x), T(p.y)) {}

// Math functions
template <typename T>
MSK_CPU_GPU inline auto dot(const Vector2<T> &v1, const Vector2<T> &v2) {
  return sum_of_prod(v1.x, v2.x, v1.y, v2.y);
}

template <typename T>
MSK_CPU_GPU inline auto abs_dot(const Vector2<T> &v1, const Vector2<T> &v2) {
  return std::abs(dot(v1, v2));
}

template <typename T>
MSK_CPU_GPU inline auto squared_norm(const Vector2<T> &v) {
  return sqr(v.x) + sqr(v.y);
}

template <typename T>
MSK_CPU_GPU inline auto norm(const Vector2<T> &v) {
  return std::sqrt(squared_norm(v));
}

template <typename T>
MSK_CPU_GPU inline auto normalize(const Vector2<T> &v) {
  return v / norm(v);
}

template <typename T>
MSK_CPU_GPU inline auto distance(const Point2<T> &p1, const Point2<T> &p2) {
  return norm(p1 - p2);
}

template <typename T>
MSK_CPU_GPU inline auto squared_distance(const Point2<T> &p1, const Point2<T> &p2) {
  return squared_norm(p1 - p2);
}

}  // namespace math
}  // namespace misaki