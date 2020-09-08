#pragma once

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "../system.h"

namespace misaki::math {

template <typename T>
class TColor3;
template <typename T>
class TColor4;
template <typename T>
class TVector2;
template <typename T>
class TVector3;
template <typename T>
class TVector4;

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr auto Pi = T(3.14159265358979323846);
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr auto InvPi = T(0.31830988618379067154);
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr auto InvTwoPi = T(0.15915494309189533577);
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr auto InvFourPi = T(0.07957747154594766788);
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr auto Infinity = std::numeric_limits<T>::infinity();
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr auto Epsilon = std::numeric_limits<T>::epsilon() / 2;

template <int N, typename T>
T power(const T &x) {
  if constexpr (N == 0) {
    return T(1);
  } else if constexpr (N % 2 == 0) {
    auto tmp = power<N / 2>(x);
    return tmp * tmp;
  } else {
    auto tmp = power<N / 2>(x);
    return tmp * tmp * x;
  }
}

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
MSK_INLINE T lerp(T a, T b, T t) {
  return (1 - t) * a + t * b;
}

template <typename T>
MSK_INLINE auto deg_to_rag(const T &v) {
  return v * T(Pi<T> / 180);
}

template <typename T>
MSK_INLINE auto rag_to_deg(const T &v) {
  return v * T(180 / Pi<T>);
}

template <typename T>
MSK_INLINE T sqr(const T &a) {
  return a * a;
}

template <typename T>
MSK_INLINE auto safe_sqrt(const T &a) {
  return std::sqrt(std::max(a, T(0)));
}

template <typename T>
MSK_INLINE auto safe_rsqrt(const T &a) {
  return T(1) / std::sqrt(std::max(a, T(0)));
}

template <typename T>
MSK_INLINE auto safe_asin(const T &a) {
  return std::asin(std::min(T(1), std::max(T(-1), a)));
}

template <typename T>
MSK_INLINE auto safe_acos(const T &a) {
  return std::acos(std::min(T(1), std::max(T(-1), a)));
}

}  // namespace misaki::math