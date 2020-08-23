#pragma once

#include "../system.h"

namespace mstd {

namespace span_internal {

// Wrappers for access to container data pointers.
template <typename C>
MSK_CPU_GPU inline constexpr auto GetDataImpl(C &c, char) noexcept
    -> decltype(c.data()) {
  return c.data();
}

template <typename C>
MSK_CPU_GPU inline constexpr auto GetData(C &c) noexcept -> decltype(GetDataImpl(c, 0)) {
  return GetDataImpl(c, 0);
}

// Detection idioms for size() and data().
template <typename C>
using HasSize =
    std::is_integral<typename std::decay<decltype(std::declval<C &>().size())>::type>;

// We want to enable conversion from vector<T*> to span<const T* const> but
// disable conversion from vector<Derived> to span<Base>. Here we use
// the fact that U** is convertible to Q* const* if and only if Q is the same
// type or a more cv-qualified version of U.  We also decay the result type of
// data() to avoid problems with classes which have a member function data()
// which returns a reference.
template <typename T, typename C>
using HasData = std::is_convertible<
    typename std::decay<decltype(GetData(std::declval<C &>()))>::type *, T *const *>;

}  // namespace span_internal

// span implementation partially based on absl::Span from Google's Abseil library.
template <typename T>
class span {
 public:
  // Used to determine whether a Span can be constructed from a container of
  // type C.
  template <typename C>
  using EnableIfConvertibleFrom =
      typename std::enable_if_t<span_internal::HasData<T, C>::value &&
                                span_internal::HasSize<C>::value>;

  // Used to SFINAE-enable a function when the slice elements are const.
  template <typename U>
  using EnableIfConstView = typename std::enable_if_t<std::is_const<T>::value, U>;

  // Used to SFINAE-enable a function when the slice elements are mutable.
  template <typename U>
  using EnableIfMutableView = typename std::enable_if_t<!std::is_const<T>::value, U>;

  using value_type = typename std::remove_cv<T>::type;
  using iterator = T *;
  using const_iterator = const T *;

  MSK_CPU_GPU
  span() : ptr(nullptr), n(0) {}
  MSK_CPU_GPU
  span(T *ptr, size_t n) : ptr(ptr), n(n) {}
  template <size_t N>
  MSK_CPU_GPU span(T (&a)[N]) : span(a, N) {}
  MSK_CPU_GPU
  span(std::initializer_list<value_type> v) : span(v.begin(), v.size()) {}

  // Explicit reference constructor for a mutable `span<T>` type. Can be
  // replaced with Makespan() to infer the type parameter.
  template <typename V, typename = EnableIfConvertibleFrom<V>,
            typename = EnableIfMutableView<V>>
  MSK_CPU_GPU explicit span(V &v) noexcept : span(v.data(), v.size()) {}

  // Implicit reference constructor for a read-only `span<const T>` type
  template <typename V, typename = EnableIfConvertibleFrom<V>,
            typename = EnableIfConstView<V>>
  MSK_CPU_GPU constexpr span(const V &v) noexcept : span(v.data(), v.size()) {}

  MSK_CPU_GPU
  iterator begin() { return ptr; }
  MSK_CPU_GPU
  iterator end() { return ptr + n; }
  MSK_CPU_GPU
  const_iterator begin() const { return ptr; }
  MSK_CPU_GPU
  const_iterator end() const { return ptr + n; }

  MSK_CPU_GPU
  T &operator[](size_t i) {
    DCHECK_LT(i, size());
    return ptr[i];
  }
  MSK_CPU_GPU
  const T &operator[](size_t i) const {
    DCHECK_LT(i, size());
    return ptr[i];
  }

  MSK_CPU_GPU
  size_t size() const { return n; };
  MSK_CPU_GPU
  bool empty() const { return size() == 0; }
  MSK_CPU_GPU
  T *data() { return ptr; }
  MSK_CPU_GPU
  const T *data() const { return ptr; }

  MSK_CPU_GPU
  T front() const { return ptr[0]; }
  MSK_CPU_GPU
  T back() const { return ptr[n - 1]; }

  MSK_CPU_GPU
  void remove_prefix(size_t count) {
    // assert(size() >= count);
    ptr += count;
    n -= count;
  }
  MSK_CPU_GPU
  void remove_suffix(size_t count) {
    // assert(size() > = count);
    n -= count;
  }

  MSK_CPU_GPU
  span subspan(size_t pos, size_t count) {
    size_t np = count < (size() - pos) ? count : (size() - pos);
    return span(ptr + pos, np);
  }

 private:
  T *ptr;
  size_t n;
};

template <int &... ExplicitArgumentBarrier, typename T>
MSK_CPU_GPU inline constexpr span<T> make_span(T *ptr, size_t size) noexcept {
  return span<T>(ptr, size);
}

template <int &... ExplicitArgumentBarrier, typename T>
MSK_CPU_GPU inline span<T> make_span(T *begin, T *end) noexcept {
  return span<T>(begin, end - begin);
}

template <int &... ExplicitArgumentBarrier, typename C>
MSK_CPU_GPU inline constexpr auto make_span(C &c) noexcept
    -> decltype(make_span(span_internal::GetData(c), c.size())) {
  return make_span(span_internal::GetData(c), c.size());
}

template <int &... ExplicitArgumentBarrier, typename T, size_t N>
MSK_CPU_GPU inline constexpr span<T> make_span(T (&array)[N]) noexcept {
  return span<T>(array, N);
}

template <int &... ExplicitArgumentBarrier, typename T>
MSK_CPU_GPU inline constexpr span<const T> make_const_span(T *ptr, size_t size) noexcept {
  return span<const T>(ptr, size);
}

template <int &... ExplicitArgumentBarrier, typename T>
MSK_CPU_GPU inline span<const T> make_const_span(T *begin, T *end) noexcept {
  return span<const T>(begin, end - begin);
}

template <int &... ExplicitArgumentBarrier, typename C>
MSK_CPU_GPU inline constexpr auto make_const_span(const C &c) noexcept
    -> decltype(make_span(c)) {
  return make_span(c);
}

template <int &... ExplicitArgumentBarrier, typename T, size_t N>
MSK_CPU_GPU inline constexpr span<const T> make_const_span(const T (&array)[N]) noexcept {
  return span<const T>(array, N);
}

}