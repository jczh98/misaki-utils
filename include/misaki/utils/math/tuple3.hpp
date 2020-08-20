#pragma once

#include "common.hpp"

namespace misaki {
namespace math {

// Tuple3
template <template <typename> class Child, typename T>
class Tuple3 {
 public:
  Tuple3() = default;

  MSK_CPU_GPU Tuple3(T x, T y, T z) : x(x), y(y), z(z) {}

  MSK_CPU_GPU bool has_nan() const { return is_nan(x) || is_nan(y) || is_nan(z); }

  // Operators
  template <typename U>
  MSK_CPU_GPU auto operator+(const Child<U> &c) const -> Child<decltype(T{} + U{})> {
    return {x + c.x, y + c.y, z + c.z};
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator+=(const Child<U> &c) const {
    x += c.x;
    y += c.y;
    z += c.z;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator-(const Child<U> &c) const -> Child<decltype(T{} - U{})> {
    return {x - c.x, y - c.y, z - c.z};
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator-=(const Child<U> &c) const {
    x -= c.x;
    y -= c.y;
    z -= c.z;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator*(const Child<U> &c) const -> Child<decltype(T{} * U{})> {
    return {x * c.x, y * c.y, z * c.z};
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator*=(const Child<U> &c) const {
    x *= c.x;
    y *= c.y;
    z *= c.z;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator*(U s) const -> Child<typename std::common_type<T, U>::type> {
    return Child<typename std::common_type<T, U>::type>(s * x, s * y, s * z);
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator*=(U s) {
    x *= s;
    y *= s;
    z *= s;
    return static_cast<Child<T> &>(*this);
  }

  template <typename U>
  MSK_CPU_GPU auto operator/(U s) const -> Child<typename std::common_type<T, U>::type> {
    return Child<typename std::common_type<T, U>::type>(x / s, y / s, z / s);
  }

  template <typename U>
  MSK_CPU_GPU Child<T> &operator/=(U s) {
    x /= s;
    y /= s;
    z /= s;
    return static_cast<Child<T> &>(*this);
  }

  MSK_CPU_GPU Child<T> operator-() const { return {-x, -y, -z}; }

  MSK_CPU_GPU T operator[](int i) const {
    return (i == 0) ? x : ((i == 1) ? y : z);
  }

  MSK_CPU_GPU T &operator[](int i) {
    return (i == 0) ? x : ((i == 1) ? y : z);
  }

  MSK_CPU_GPU bool operator==(const Child<T> &c) const { return x == c.x && y == c.y && z == c.z; }
  MSK_CPU_GPU bool operator!=(const Child<T> &c) const { return x != c.x || y != c.y || z != c.z; }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }

  T x{}, y{}, z{};
};

// Stream
template <template <class> class C, typename T>
std::ostream &operator<<(
    std::ostream &oss, const Tuple3<C, T> &vec) {
  oss << "[";
  oss << vec.x << ", ";
  oss << vec.y << ", ";
  oss << vec.z;
  oss << "]";
  return oss;
}

// Inline functions
template <template <class> class C, typename T, typename U>
MSK_CPU_GPU inline auto operator*(U s, const Tuple3<C, T> &t)
    -> C<typename std::common_type<T, U>::type> {
  return t * s;
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> abs(const Tuple3<C, T> &t) {
  return {std::abs(t.x), std::abs(t.y), std::abs(t.z)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> ceil(const Tuple3<C, T> &t) {
  return {std::ceil(t.x), std::ceil(t.y), std::ceil(t.z)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> floor(const Tuple3<C, T> &t) {
  return {std::floor(t.x), std::floor(t.y), std::floor(t.z)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline auto lerp(T t, const Tuple3<C, T> &t0, const Tuple3<C, T> &t1) {
  return (1 - t) * t0 + t * t1;
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> min(const Tuple3<C, T> &t0, const Tuple3<C, T> &t1) {
  return {std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline T hmin(const Tuple3<C, T> &t) {
  return std::min({t.x, t.y, t.z});
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline C<T> max(const Tuple3<C, T> &t0, const Tuple3<C, T> &t1) {
  return {std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z)};
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline T hmax(const Tuple3<C, T> &t) {
  return std::max({t.x, t.y, t.z});
}

template <template <class> class C, typename T>
MSK_CPU_GPU inline T hprod(const Tuple3<C, T> &t) {
  return t.x * t.y * t.z;
}

// Vector3 definition
template <typename T>
class Vector3 : public Tuple3<Vector3, T> {
 public:
  // Vector3 Public Methods
  using Tuple3<Vector3, T>::x;
  using Tuple3<Vector3, T>::y;
  using Tuple3<Vector3, T>::z;

  Vector3() = default;
  MSK_CPU_GPU Vector3(T x, T y, T z) : Tuple3<Vector3, T>(x, y, z) {}

  template <typename U>
  MSK_CPU_GPU explicit Vector3(const Point3<U> &p);
  template <typename U>
  MSK_CPU_GPU explicit Vector3(const Normal3<U> &p);

  template <typename U>
  MSK_CPU_GPU explicit Vector3(const Vector3<U> &v)
      : Tuple3<Vector3, T>(T(v.x), T(v.y), T(v.z)) {}
};

// Point3 definition
template <typename T>
class Point3 : public Tuple3<Point3, T> {
 public:
  // Point3 Public Methods
  using Tuple3<Point3, T>::x;
  using Tuple3<Point3, T>::y;
  using Tuple3<Point3, T>::z;
  using Tuple3<Point3, T>::has_nan;
  using Tuple3<Point3, T>::operator+;
  using Tuple3<Point3, T>::operator+=;
  using Tuple3<Point3, T>::operator*;
  using Tuple3<Point3, T>::operator*=;

  MSK_CPU_GPU Point3() { x = y = z = 0; }

  MSK_CPU_GPU Point3(T x, T y, T z) : Tuple3<Point3, T>(x, y, z) {}

  template <typename U>
  MSK_CPU_GPU explicit Point3(const Point3<U> &v)
      : Tuple3<Point3, T>(T(v.x), T(v.y), T(v.z)) {}

  template <typename U>
  MSK_CPU_GPU explicit Point3(const Vector3<U> &v)
      : Tuple3<Point3, T>(T(v.x), T(v.y), T(v.z)) {}

  template <typename U>
  MSK_CPU_GPU auto operator+(const Vector3<U> &v) const
      -> Point3<decltype(T{} + U{})> {
    return {x + v.x, y + v.y, z + v.z};
  }

  template <typename U>
  MSK_CPU_GPU Point3<T> &operator+=(const Vector3<U> &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  MSK_CPU_GPU Point3<T> operator-() const { return {-x, -y, -z}; }

  template <typename U>
  MSK_CPU_GPU auto operator-(const Point3<U> &p) const
      -> Vector3<decltype(T{} - U{})> {
    return {x - p.x, y - p.y, z - p.z};
  }

  template <typename U>
  MSK_CPU_GPU auto operator-(const Vector3<U> &v) const
      -> Point3<decltype(T{} - U{})> {
    return {x - v.x, y - v.y, z - v.z};
  }

  template <typename U>
  MSK_CPU_GPU Point3<T> &operator-=(const Vector3<U> &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
};

// Normal3 definition
template <typename T>
class Normal3 : public Tuple3<Normal3, T> {
 public:
  using Tuple3<Normal3, T>::x;
  using Tuple3<Normal3, T>::y;
  using Tuple3<Normal3, T>::z;
  using Tuple3<Normal3, T>::has_nan;
  using Tuple3<Normal3, T>::operator+;
  using Tuple3<Normal3, T>::operator*;
  using Tuple3<Normal3, T>::operator*=;

  Normal3() = default;

  MSK_CPU_GPU Normal3(T x, T y, T z) : Tuple3<Normal3, T>(x, y, z) {}

  template <typename U>
  MSK_CPU_GPU explicit Normal3<T>(const Normal3<U> &v)
      : Tuple3<Normal3, T>(T(v.x), T(v.y), T(v.z)) {}

  template <typename U>
  MSK_CPU_GPU explicit Normal3<T>(const Vector3<U> &v)
      : Tuple3<Normal3, T>(T(v.x), T(v.y), T(v.z)) {}
};

// Vector3 construction
template <typename T>
template <typename U>
Vector3<T>::Vector3(const Point3<U> &p) : Tuple3<Vector3, T>(T(p.x), T(p.y), T(p.z)) {}

template <typename T>
template <typename U>
Vector3<T>::Vector3(const Normal3<U> &p) : Tuple3<Vector3, T>(T(p.x), T(p.y), T(p.z)) {}

// Math functions for Vector3
template <typename T>
MSK_CPU_GPU inline Vector3<T> cross(const Vector3<T> &v1, const Normal3<T> &v2) {
  return {diff_of_prod(v1.y, v2.z, v1.z, v2.y),
          diff_of_prod(v1.z, v2.x, v1.x, v2.z),
          diff_of_prod(v1.x, v2.y, v1.y, v2.x)};
}

template <typename T>
MSK_CPU_GPU inline Vector3<T> cross(const Normal3<T> &v1, const Vector3<T> &v2) {
  return {diff_of_prod(v1.y, v2.z, v1.z, v2.y),
          diff_of_prod(v1.z, v2.x, v1.x, v2.z),
          diff_of_prod(v1.x, v2.y, v1.y, v2.x)};
}

template <typename T>
MSK_CPU_GPU inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) {
  return {diff_of_prod(v1.y, v2.z, v1.z, v2.y),
          diff_of_prod(v1.z, v2.x, v1.x, v2.z),
          diff_of_prod(v1.x, v2.y, v1.y, v2.x)};
}

template <typename T>
MSK_CPU_GPU inline auto dot(const Vector3<T> &v1, const Vector3<T> &v2) {
  return fmadd(v1.x, v2.x, sum_of_prod(v1.y, v2.y, v1.z, v2.z));
}

template <typename T>
MSK_CPU_GPU inline auto abs_dot(const Vector3<T> &v1, const Vector3<T> &v2) {
  return std::abs(dot(v1, v2));
}

template <typename T>
MSK_CPU_GPU inline auto squared_norm(const Vector3<T> &v) {
  return sqr(v.x) + sqr(v.y) + sqr(v.z);
}

template <typename T>
MSK_CPU_GPU inline auto norm(const Vector3<T> &v) {
  return std::sqrt(squared_norm(v));
}

template <typename T>
MSK_CPU_GPU inline auto normalize(const Vector3<T> &v) {
  return v / norm(v);
}

// Compute orthonormal basis
// Based on paper "Building an Orthonormal Basis, Revisited"
template <typename T>
MSK_CPU_GPU inline std::pair<Vector3<T>, Vector3<T>> coordinate_system(const Vector3<T> &v1) {
  Float sign = std::copysign(Float(1), v1.z);
  Float a = -1 / (sign + v1.z);
  Float b = v1.x * v1.y * a;
  return {{1 + sign * v1.x * v1.x * a, sign * b, -sign * v1.x},
          {b, sign + v1.y * v1.y * a, -v1.y}};
}

template <typename T>
MSK_CPU_GPU inline std::pair<Vector3<T>, Vector3<T>> coordinate_system(const Normal3<T> &v1) {
  Float sign = std::copysign(Float(1), v1.z);
  Float a = -1 / (sign + v1.z);
  Float b = v1.x * v1.y * a;
  return {{1 + sign * v1.x * v1.x * a, sign * b, -sign * v1.x},
          {b, sign + v1.y * v1.y * a, -v1.y}};
}

// Math functions for Point3 and Normal3
template <typename T>
MSK_CPU_GPU inline auto distance(const Point3<T> &p1, const Point3<T> &p2) {
  return norm(p1 - p2);
}

template <typename T>
MSK_CPU_GPU inline auto squared_distance(const Point3<T> &p1, const Point3<T> &p2) {
  return squared_norm(p1 - p2);
}

template <typename T>
MSK_CPU_GPU inline auto squared_norm(const Normal3<T> &n) {
  return sqr(n.x) + sqr(n.y) + sqr(n.z);
}

template <typename T>
MSK_CPU_GPU inline auto norm(const Normal3<T> &n) {
  return std::sqrt(squared_norm(n));
}

template <typename T>
MSK_CPU_GPU inline Normal3<T> normalize(const Normal3<T> &n) {
  return n / norm(n);
}

template <typename T>
MSK_CPU_GPU inline auto dot(const Normal3<T> &n, const Vector3<T> &v) {
  return fmadd(n.x, v.x, sum_of_prod(n.y, v.y, n.z, v.z));
}

template <typename T>
MSK_CPU_GPU inline auto dot(const Vector3<T> &v, const Normal3<T> &n) {
  return fmadd(n.x, v.x, sum_of_prod(n.y, v.y, n.z, v.z));
}

template <typename T>
MSK_CPU_GPU inline auto dot(const Normal3<T> &n1, const Normal3<T> &n2) {
  return fmadd(n1.x, n2.x, sum_of_prod(n1.y, n2.y, n1.z, n2.z));
}

template <typename T>
MSK_CPU_GPU inline auto abs_dot(const Normal3<T> &n, const Vector3<T> &v) {
  return std::abs(dot(n, v));
}

template <typename T>
MSK_CPU_GPU inline auto abs_dot(const Vector3<T> &v, const Normal3<T> &n) {
  return std::abs(dot(v, n));
}

template <typename T>
MSK_CPU_GPU inline auto abs_dot(const Normal3<T> &n1, const Normal3<T> &n2) {
  return std::abs(dot(n1, n2));
}

template <typename T>
MSK_CPU_GPU inline Normal3<T> face_forward(const Normal3<T> &n, const Vector3<T> &v) {
  return (dot(n, v) < 0.f) ? -n : n;
}

template <typename T>
MSK_CPU_GPU inline Normal3<T> face_forward(const Normal3<T> &n, const Normal3<T> &n2) {
  return (dot(n, n2) < 0.f) ? -n : n;
}

template <typename T>
MSK_CPU_GPU inline Vector3<T> face_forward(const Vector3<T> &v, const Vector3<T> &v2) {
  return (dot(v, v2) < 0.f) ? -v : v;
}

template <typename T>
MSK_CPU_GPU inline Vector3<T> face_forward(const Vector3<T> &v, const Normal3<T> &n2) {
  return (dot(v, n2) < 0.f) ? -v : v;
}

}  // namespace math
}  // namespace misaki