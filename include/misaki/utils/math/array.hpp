#pragma once

#include <cmath>
#include <cstddef>
#include <sstream>
#include <type_traits>

#include "../system/platform.h"

namespace misaki::math {

// Predifinitoin math
MSK_XPU inline float select(bool c, float a, float b) { return c ? a : b; }
MSK_XPU inline int select(bool c, int a, int b) { return c ? a : b; }
MSK_XPU inline double select(bool c, double a, double b) { return c ? a : b; }
MSK_XPU inline bool any(bool a) { return a; }
MSK_XPU inline bool any(float a) { return a; }
MSK_XPU inline bool any(double a) { return a; }
MSK_XPU inline bool all(bool a) { return a; }
MSK_XPU inline bool all(float a) { return a; }
MSK_XPU inline bool all(double a) { return a; }
MSK_XPU inline auto min(float a, float b) { return std::min(a, b); }
MSK_XPU inline auto min(double a, double b) { return std::min(a, b); }
MSK_XPU inline auto min(int a, int b) { return std::min(a, b); }
MSK_XPU inline auto max(float a, float b) { return std::max(a, b); }
MSK_XPU inline auto max(double a, double b) { return std::max(a, b); }
MSK_XPU inline auto max(int a, int b) { return std::max(a, b); }

template <typename Value_, size_t Size_, typename Derived_>
class StaticArrayBase {
 public:
  using Value = Value_;
  using Derived = Derived_;

  static constexpr size_t Size = Size_;

  using MaskType = StaticArrayBase<bool, Size_, Derived_>;

  MSK_XPU Derived &derived() { return (Derived &)*this; }
  MSK_XPU const Derived &derived() const { return (Derived &)*this; }

  StaticArrayBase() = default;
  MSK_XPU StaticArrayBase(const Value &v1) {
    for (int i = 0; i < Size; ++i) coeff(i) = v1;
  }
  MSK_XPU StaticArrayBase(const Value &v1, const Value &v2) {
    static_assert(Size >= 2,
                  "StaticArrayBase::constructor(): requires Size >= 2");
    coeff(0) = v1;
    coeff(1) = v2;
  }

  MSK_XPU StaticArrayBase(const Value &v1, const Value &v2, const Value &v3) {
    static_assert(Size >= 3,
                  "StaticArrayBase::constructor(): requires Size >= 3");
    coeff(0) = v1;
    coeff(1) = v2;
    coeff(2) = v3;
  }

  MSK_XPU StaticArrayBase(const Value &v1, const Value &v2, const Value &v3,
                          const Value &v4) {
    static_assert(Size >= 4,
                  "StaticArrayBase::constructor(): requires Size >= 4");
    coeff(0) = v1;
    coeff(1) = v2;
    coeff(2) = v3;
    coeff(3) = v4;
  }

  // Type cast
  template <typename Value2, size_t Size2, typename Derived2,
            typename T = Derived,
            std::enable_if_t<Derived2::Size == T::Size, int> = 0>
  MSK_XPU StaticArrayBase(const StaticArrayBase<Value2, Size2, Derived2> &a) {
    for (size_t i = 0; i < Derived::Size; ++i)
      (Value &)derived().coeff(i) = (const Value &)a.derived().coeff(i);
  }

#define GEN_ARITH_OP(op, assign_op)                         \
  MSK_XPU Derived operator op(const Derived &rhs) const {   \
    Derived ret;                                            \
    for (int i = 0; i < Size; ++i) {                        \
      ret.coeff(i) = coeff(i) op rhs.coeff(i);              \
    }                                                       \
    return ret;                                             \
  }                                                         \
  MSK_XPU Derived operator op(const Value &rhs) const {     \
    Derived ret;                                            \
    for (int i = 0; i < Size; ++i) {                        \
      ret.coeff(i) = coeff(i) op rhs;                       \
    }                                                       \
    return ret;                                             \
  }                                                         \
  MSK_XPU Derived &operator assign_op(const Derived &rhs) { \
    *this = *this op rhs;                                   \
  }                                                         \
  MSK_XPU Derived &operator assign_op(const Value &rhs) {   \
    *this = *this op rhs;                                   \
  }

  GEN_ARITH_OP(+, +=)
  GEN_ARITH_OP(-, -=)
  GEN_ARITH_OP(*, *=)
  GEN_ARITH_OP(/, /=)
#undef GEN_ARITH_OP

  MSK_XPU friend Derived operator+(const Value &v, const Derived &rhs) {
    return Derived(v) + rhs;
  }
  MSK_XPU friend Derived operator-(const Value &v, const Derived &rhs) {
    return Derived(v) - rhs;
  }
  MSK_XPU friend Derived operator*(const Value &v, const Derived &rhs) {
    return Derived(v) * rhs;
  }
  MSK_XPU friend Derived operator/(const Value &v, const Derived &rhs) {
    return Derived(v) / rhs;
  }

#define GEN_CMP_OP(op)                                                      \
  MSK_XPU MaskType operator op(const Derived &rhs) const {                  \
    MaskType ret;                                                           \
    for (int i = 0; i < Size; ++i) ret.coeff(i) = coeff(i) op rhs.coeff(i); \
    return ret;                                                             \
  }
  GEN_CMP_OP(==)
  GEN_CMP_OP(!=)
  GEN_CMP_OP(<=)
  GEN_CMP_OP(>=)
  GEN_CMP_OP(<)
  GEN_CMP_OP(>)
#undef GEN_CMP_OP

  MSK_XPU Derived operator-() const {
    Derived self;
    for (int i = 0; i < Size; i++) {
      self.coeff(i) = -coeff(i);
    }
    return self;
  }

  // Component acess
  MSK_XPU Value &coeff(size_t i) { return m_data[i]; }
  MSK_XPU const Value &coeff(size_t i) const { return m_data[i]; }

  // Recursive array indexing operator (const)
  template <typename... Args, std::enable_if_t<(sizeof...(Args) >= 1), int> = 0>
  MSK_XPU decltype(auto) coeff(size_t i0, Args... other) const {
    return coeff(i0).coeff(size_t(other)...);
  }

  // Recursive array indexing operator
  template <typename... Args, std::enable_if_t<(sizeof...(Args) >= 1), int> = 0>
  MSK_XPU decltype(auto) coeff(size_t i0, Args... other) {
    return coeff(i0).coeff(size_t(other)...);
  }

  MSK_XPU decltype(auto) x() const {
    static_assert(Size >= 1, "StaticArrayBase::x(): requires Size >= 1");
    return coeff(0);
  }

  MSK_XPU decltype(auto) x() {
    static_assert(Size >= 1, "StaticArrayBase::x(): requires Size >= 1");
    return coeff(0);
  }

  MSK_XPU decltype(auto) y() const {
    static_assert(Size >= 2, "StaticArrayBase::y(): requires Size >= 2");
    return coeff(1);
  }

  MSK_XPU decltype(auto) y() {
    static_assert(Size >= 2, "StaticArrayBase::y(): requires Size >= 2");
    return coeff(1);
  }

  MSK_XPU decltype(auto) z() const {
    static_assert(Size >= 3, "StaticArrayBase::z(): requires Size >= 3");
    return coeff(2);
  }

  MSK_XPU decltype(auto) z() {
    static_assert(Size >= 3, "StaticArrayBase::z(): requires Size >= 3");
    return coeff(2);
  }

  MSK_XPU decltype(auto) w() const {
    static_assert(Size >= 4, "StaticArrayBase::w(): requires Size >= 4");
    return coeff(3);
  }

  MSK_XPU decltype(auto) w() {
    static_assert(Size >= 4, "StaticArrayBase::w(): requires Size >= 4");
    return coeff(3);
  }

 private:
  Value m_data[Size] = {0};
};

// Array math
template <typename Value, size_t Size, typename Derived>
MSK_XPU Value dot(const StaticArrayBase<Value, Size, Derived> &a1,
                  const StaticArrayBase<Value, Size, Derived> &a2) {
  Value s = a1.coeff(0) * a2.coeff(0);
  for (int i = 1; i < Size; i++) {
    s += a1.coeff(i) * a2.coeff(i);
  }
  return s;
}

template <typename Value, size_t Size, typename Derived, class F>
MSK_XPU Value reduce(const StaticArrayBase<Value, Size, Derived> &a, F &&f) {
  Value acc = a.coeff(0);
  for (int i = 1; i < Size; ++i) acc = f(acc, a.coeff(i));
  return acc;
}

template <typename Value, size_t Size, typename Derived>
MSK_XPU Value hsum(const StaticArrayBase<Value, Size, Derived> &a) {
  return reduce(a, [](const Value &acc, const Value &b) { return acc + b; });
}

template <typename Value, size_t Size, typename Derived>
MSK_XPU Value hprod(const StaticArrayBase<Value, Size, Derived> &a) {
  return reduce(a, [](const Value &acc, const Value &b) { return acc + b; });
}

template <typename Value, size_t Size, typename Derived>
MSK_XPU Value hmin(const StaticArrayBase<Value, Size, Derived> &a) {
  return reduce(a,
                [](const Value &acc, const Value &b) { return min(acc, b); });
}

template <typename Value, size_t Size, typename Derived>
MSK_XPU Value hmax(const StaticArrayBase<Value, Size, Derived> &a) {
  return reduce(a,
                [](const Value &acc, const Value &b) { return max(acc, b); });
}

template <size_t Size, typename Derived>
MSK_XPU bool any(const StaticArrayBase<bool, Size, Derived> &a) {
  return reduce(a,
                [](const bool &acc, const bool &b) { return acc || any(b); });
}

template <size_t Size, typename Derived>
MSK_XPU bool all(const StaticArrayBase<bool, Size, Derived> &a) {
  return reduce(a,
                [](const bool &acc, const bool &b) { return acc && all(b); });
}

template <typename Value, size_t Size, typename Derived>
MSK_XPU Derived select(const StaticArrayBase<bool, Size, Derived> &x,
                       const StaticArrayBase<Value, Size, Derived> &a,
                       const StaticArrayBase<Value, Size, Derived> &b) {
  Derived ret;
  for (int i = 0; i < Size; i++) {
    ret.coeff(i) = select(x.coeff(i), a.coeff(i), b.coeff(i));
  }
  return ret;
}

template <typename Value, size_t Size, typename Derived>
std::ostream &operator<<(std::ostream &oss,
                         const StaticArrayBase<Value, Size, Derived> &a) {
  oss << "[";
  for (int i = 0; i < Size - 1; ++i) oss << a.coeff(i) << ", ";
  oss << a.coeff(Size - 1);
  oss << "]";
  return oss;
}

#define MSK_ARRAY_IMPORT_BASIC(Base, Array)  \
  Array(const Array &) = default;            \
  Array(Array &&) = default;                 \
  Array &operator=(const Array &) = default; \
  Array &operator=(Array &&) = default;      \
  using typename Base::Value;                \
  using typename Base::Derived;              \
  using Base::Size;                          \
  using Base::derived;

#define MSK_ARRAY_IMPORT(Base, Array) \
  MSK_ARRAY_IMPORT_BASIC(Base, Array) \
  using Base::Base;                   \
  using Base::operator=;

template <typename Value_, size_t Size_>
class Array : public StaticArrayBase<Value_, Size_, Array<Value_, Size_>> {
 public:
  using Base = StaticArrayBase<Value_, Size_, Array<Value_, Size_>>;
  MSK_ARRAY_IMPORT(Base, Array)
};

}  // namespace misaki::math