#pragma once

#include "../system.h"

namespace misaki::math {

template <typename T>
constexpr auto Pi = T(3.14159265358979323846);
template <typename T>
constexpr auto InvPi = T(0.31830988618379067154);
template <typename T>
constexpr auto InvTwoPi = T(0.15915494309189533577);
template <typename T>
constexpr auto InvFourPi = T(0.07957747154594766788);

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

template <typename T>
T clamp(T value, T vmin, T vmax) {
  return value < vmin ? vmin : (value > vmax ? vmax : value);
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

template <typename Predicate>
uint32_t find_interval(uint32_t size, const Predicate &pred) {
  int first = 0, len = size;
  while (len > 0) {
    int half = len >> 1, middle = first + half;
    if (pred(middle)) {
      first = middle + 1;
      len -= half + 1;
    } else
      len = half;
  }
  return std::clamp<int>(first - 1, 0, size - 2);
}

// Search the interval of [begin, end)
template <typename Predicate>
uint32_t binary_search(uint32_t begin, uint32_t end, const Predicate &pred) {
  while (begin < end) {
    uint32_t middle = begin + (end - begin) >> 1;
    if (pred(middle)) {
      begin = middle + 1;
    } else
      end = middle;
  }
  return begin;
}

}  // namespace misaki::math