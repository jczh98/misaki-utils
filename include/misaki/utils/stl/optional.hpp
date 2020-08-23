#pragma once

#include "../system.h"

namespace mstd {

template <typename T>
class optional {
 public:
  using value_type = T;

  optional() = default;
  MSK_CPU_GPU
  optional(const T &v) : set(true) { new (ptr()) T(v); }
  MSK_CPU_GPU
  optional(T &&v) : set(true) { new (ptr()) T(std::move(v)); }
  MSK_CPU_GPU
  optional(const optional &v) : set(v.has_value()) {
    if (v.has_value())
      new (ptr()) T(v.value());
  }
  MSK_CPU_GPU
  optional(optional &&v) : set(v.has_value()) {
    if (v.has_value()) {
      new (ptr()) T(std::move(v.value()));
      v.reset();
    }
  }

  MSK_CPU_GPU
  optional &operator=(const T &v) {
    reset();
    new (ptr()) T(v);
    set = true;
    return *this;
  }
  MSK_CPU_GPU
  optional &operator=(T &&v) {
    reset();
    new (ptr()) T(std::move(v));
    set = true;
    return *this;
  }
  MSK_CPU_GPU
  optional &operator=(const optional &v) {
    reset();
    if (v.has_value()) {
      new (ptr()) T(v.value());
      set = true;
    }
    return *this;
  }
  MSK_CPU_GPU
  optional &operator=(optional &&v) {
    reset();
    if (v.has_value()) {
      new (ptr()) T(std::move(v.value()));
      set = true;
      v.reset();
    }
    return *this;
  }

  MSK_CPU_GPU
  ~optional() { reset(); }

  MSK_CPU_GPU
  explicit operator bool() const { return set; }

  MSK_CPU_GPU
  T value_or(const T &alt) const { return set ? value() : alt; }

  MSK_CPU_GPU
  T *operator->() { return &value(); }
  MSK_CPU_GPU
  const T *operator->() const { return &value(); }
  MSK_CPU_GPU
  T &operator*() { return value(); }
  MSK_CPU_GPU
  const T &operator*() const { return value(); }
  MSK_CPU_GPU
  T &value() {
    CHECK(set);
    return *ptr();
  }
  MSK_CPU_GPU
  const T &value() const {
    CHECK(set);
    return *ptr();
  }

  MSK_CPU_GPU
  void reset() {
    if (set) {
      value().~T();
      set = false;
    }
  }

  MSK_CPU_GPU
  bool has_value() const { return set; }

 private:
#ifdef __NVCC__
  // Work-around NVCC bug
  MSK_CPU_GPU
  T *ptr() { return reinterpret_cast<T *>(&optionalValue); }
  MSK_CPU_GPU
  const T *ptr() const { return reinterpret_cast<const T *>(&optionalValue); }
#else
  MSK_CPU_GPU
  T *ptr() { return std::launder(reinterpret_cast<T *>(&optionalValue)); }
  MSK_CPU_GPU
  const T *ptr() const {
    return std::launder(reinterpret_cast<const T *>(&optionalValue));
  }
#endif

  std::aligned_storage_t<sizeof(T), alignof(T)> optionalValue;
  bool set = false;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const optional<T> &opt) {
  if (opt.has_value())
    return os << "[ mstd::optional<" << typeid(T).name() << "> set: true "
              << "value: " << opt.value() << " ]";
  else
    return os << "[ mstd::optional<" << typeid(T).name()
              << "> set: false value: n/a ]";
}
}