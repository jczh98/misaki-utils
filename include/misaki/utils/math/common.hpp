#pragma once

#include "../system.h"

namespace misaki {
namespace math {

template <typename T>
class Vector2;
template <typename T>
class Vector3;
template <typename T>
class Point3;
template <typename T>
class Point2;
template <typename T>
class Normal3;

// Check for nans
template <typename T>
MSK_CPU_GPU inline typename std::enable_if_t<std::is_floating_point<T>::value, bool>
is_nan(const T x) {
  return std::isnan(x);
}

template <typename T>
MSK_CPU_GPU inline typename std::enable_if_t<std::is_integral<T>::value, bool> is_nan(
    const T x) {
  return false;
}

// Math functions
MSK_CPU_GPU inline float safe_sqrt(float x) {
  return std::sqrt(std::max(0.f, x));
}

MSK_CPU_GPU inline double safe_sqrt(double x) {
  return std::sqrt(std::max(0., x));
}

template <typename T>
MSK_CPU_GPU inline constexpr T sqr(T v) {
  return v * v;
}

// FMA
MSK_CPU_GPU
inline float fmadd(float a, float b, float c) {
  return std::fma(a, b, c);
}

MSK_CPU_GPU
inline double fmadd(double a, double b, double c) {
  return std::fma(a, b, c);
}

inline long double fmadd(long double a, long double b, long double c) {
  return std::fma(a, b, c);
}

// Compute without errors
template <typename Ta, typename Tb, typename Tc, typename Td>
MSK_CPU_GPU inline auto diff_of_prod(Ta a, Tb b, Tc c, Td d) {
  auto cd = c * d;
  auto err = fmadd(-c, d, cd);  // Error (exact)
  auto dop = fmadd(a, b, -cd);
  return dop + err;
}

template <typename Ta, typename Tb, typename Tc, typename Td>
MSK_CPU_GPU inline auto sum_of_prod(Ta a, Tb b, Tc c, Td d) {
  auto cd = c * d;
  auto err = fmadd(c, d, -cd);  // Error (exact)
  auto sop = fmadd(a, b, cd);
  return sop + err;
}

}  // namespace math
}  // namespace misaki