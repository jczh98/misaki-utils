#pragma once

#include "../system.h"

namespace mstd {

template <typename T>
MSK_CPU_GPU inline void swap(T &a, T &b) {
  T tmp = std::move(a);
  a = std::move(b);
  b = std::move(tmp);
}

template <typename T, int N>
class array {
 public:
  using value_type = T;
  using iterator = value_type *;
  using const_iterator = const value_type *;
  using size_t = std::size_t;

  array() = default;
  MSK_CPU_GPU
  array(std::initializer_list<T> v) {
    size_t i = 0;
    for (const T &val : v)
      values[i++] = val;
  }

  MSK_CPU_GPU
  void fill(const T &v) {
    for (int i = 0; i < N; ++i)
      values[i] = v;
  }

  MSK_CPU_GPU
  bool operator==(const array<T, N> &a) const {
    for (int i = 0; i < N; ++i)
      if (values[i] != a.values[i])
        return false;
    return true;
  }
  MSK_CPU_GPU
  bool operator!=(const array<T, N> &a) const { return !(*this == a); }

  MSK_CPU_GPU
  iterator begin() { return values; }
  MSK_CPU_GPU
  iterator end() { return values + N; }
  MSK_CPU_GPU
  const_iterator begin() const { return values; }
  MSK_CPU_GPU
  const_iterator end() const { return values + N; }

  MSK_CPU_GPU
  size_t size() const { return N; }

  MSK_CPU_GPU
  T &operator[](size_t i) { return values[i]; }
  MSK_CPU_GPU
  const T &operator[](size_t i) const { return values[i]; }

  MSK_CPU_GPU
  T *data() { return values; }
  MSK_CPU_GPU
  const T *data() const { return values; }

 private:
  T values[N] = {};
};
}