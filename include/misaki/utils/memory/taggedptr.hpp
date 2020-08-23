#pragma once

#include "../container.hpp"
#include "../system.h"
#include "../util.h"

namespace misaki::memory {

namespace detail {

using namespace misaki::container;

template <typename... Types>
struct GetTypeIndex;

template <typename T, typename... Types>
struct GetTypeIndex<T, T, Types...> {
  static const size_t value = 1;
};

template <typename T, typename U, typename... Types>
struct GetTypeIndex<T, U, Types...> {
  static const size_t value = 1 + GetTypeIndex<T, Types...>::value;
};

template <int n>
struct DispatchSplit;

template <>
struct DispatchSplit<1> {
  template <typename F, typename Tp, typename... Ts>
  MSK_CPU_GPU inline auto operator()(F func, Tp tp, int tag, TypePack<Ts...> types) {
    DCHECK_EQ(1, tag);
    static_assert(sizeof...(Ts) == 1);
    using T = typename GetFirst<TypePack<Ts...>>::type;
    return func(tp.template cast<T>());
  }
};

template <int n>
struct DispatchSplit {
  template <typename F, typename Tp, typename... Ts>
  MSK_CPU_GPU inline auto operator()(F func, Tp tp, int tag, TypePack<Ts...> types) {
    constexpr int mid = n / 2;

    if (tag - 1 < mid)  // 0-based indexing here to be more traditional
      return DispatchSplit<mid>()(
          func, tp, tag, typename TakeFirstN<mid, TypePack<Ts...>>::type());
    else
      return DispatchSplit<n - mid>()(
          func, tp, tag - mid, typename RemoveFirstN<mid, TypePack<Ts...>>::type());
  }
};

// FIXME: can we at least DispatchCRef this from the caller and dispatch based on
// whether F's return type is a const reference?
//
// https://stackoverflow.com/a/41538114 :-p

template <int n>
struct DispatchSplitCRef;

template <>
struct DispatchSplitCRef<1> {
  template <typename F, typename Tp, typename... Ts>
  MSK_CPU_GPU inline auto operator()(F func, Tp tp, int tag, TypePack<Ts...> types)
      -> auto && {
    DCHECK_EQ(1, tag);
    static_assert(sizeof...(Ts) == 1);
    using T = typename GetFirst<TypePack<Ts...>>::type;
    return func(tp.template cast<T>());
  }
};

template <int n>
struct DispatchSplitCRef {
  template <typename F, typename Tp, typename... Ts>
  MSK_CPU_GPU inline auto operator()(F func, Tp tp, int tag, TypePack<Ts...> types)
      -> auto && {
    constexpr int mid = n / 2;

    if (tag - 1 < mid)  // 0-based indexing here to be more traditional
      return DispatchSplitCRef<mid>()(
          func, tp, tag, typename TakeFirstN<mid, TypePack<Ts...>>::type());
    else
      return DispatchSplitCRef<n - mid>()(
          func, tp, tag - mid, typename RemoveFirstN<mid, TypePack<Ts...>>::type());
  }
};

template <int n>
struct DispatchSplitCPU;

template <>
struct DispatchSplitCPU<1> {
  template <typename F, typename Tp, typename... Ts>
  inline auto operator()(F func, Tp tp, int tag, TypePack<Ts...> types) {
    DCHECK_EQ(1, tag);
    static_assert(sizeof...(Ts) == 1);

    using T = typename GetFirst<TypePack<Ts...>>::type;
    return func(tp.template cast<T>());
  }
};

template <int n>
struct DispatchSplitCPU {
  template <typename F, typename Tp, typename... Ts>
  inline auto operator()(F func, Tp tp, int tag, TypePack<Ts...> types) {
    constexpr int mid = n / 2;

    if (tag - 1 < mid)  // 0-based indexing here to be more traditional
      return DispatchSplitCPU<mid>()(
          func, tp, tag, typename TakeFirstN<mid, TypePack<Ts...>>::type());
    else
      return DispatchSplitCPU<n - mid>()(
          func, tp, tag - mid, typename RemoveFirstN<mid, TypePack<Ts...>>::type());
  }
};

}  // namespace detail

template <typename... Ts>
class TaggedPointer {
 public:
  using Types = container::TypePack<Ts...>;

  TaggedPointer() = default;
  template <typename T>
  MSK_CPU_GPU TaggedPointer(T *ptr) {
    uintptr_t iptr = reinterpret_cast<uintptr_t>(ptr);
    DCHECK_EQ(iptr & PtrMask, iptr);
    constexpr uint16_t type = get_type_index<T>();
    m_bits = iptr | ((uintptr_t)type << TagShift);
  }

  MSK_CPU_GPU TaggedPointer(std::nullptr_t np) {}

  MSK_CPU_GPU TaggedPointer(const TaggedPointer &t) { m_bits = t.m_bits; }
  MSK_CPU_GPU TaggedPointer &operator=(const TaggedPointer &t) {
    m_bits = t.m_bits;
    return *this;
  }

  template <typename T>
  MSK_CPU_GPU bool is() const {
    return tag() == get_type_index<T>();
  }

  MSK_CPU_GPU explicit operator bool() const { return (m_bits & PtrMask) != 0; }

  MSK_CPU_GPU bool operator<(const TaggedPointer &tp) const { return m_bits < tp.m_bits; }

  template <typename T>
  MSK_CPU_GPU T *cast() {
    DCHECK(is<T>());
    return reinterpret_cast<T *>(ptr());
  }
  template <typename T>
  MSK_CPU_GPU const T *cast() const {
    DCHECK(is<T>());
    return reinterpret_cast<const T *>(ptr());
  }
  template <typename T>
  MSK_CPU_GPU T *cast_or_nullptr() {
    if (is<T>())
      return reinterpret_cast<T *>(ptr());
    else
      return nullptr;
  }
  template <typename T>
  MSK_CPU_GPU const T *cast_or_nullptr() const {
    if (is<T>())
      return reinterpret_cast<const T *>(ptr());
    else
      return nullptr;
  }

  MSK_CPU_GPU uint16_t tag() const { return uint16_t((m_bits & TagMask) >> TagShift); }
  MSK_CPU_GPU static constexpr uint16_t max_tag() { return sizeof...(Ts); }
  MSK_CPU_GPU static constexpr uint16_t num_tags() { return max_tag() + 1; }

  template <typename T>
  MSK_CPU_GPU static constexpr uint16_t get_type_index() {
    return uint16_t(detail::GetTypeIndex<T, Ts...>::value);
  }

  std::string to_string() const {
    return fmt::format("[ TaggedPointer ptr: 0x{} tag: {} ]", ptr(), Tag());
  }

  MSK_CPU_GPU bool operator==(const TaggedPointer &tp) const { return m_bits == tp.m_bits; }
  MSK_CPU_GPU bool operator!=(const TaggedPointer &tp) const { return m_bits != tp.m_bits; }

  MSK_CPU_GPU void *ptr() { return reinterpret_cast<void *>(m_bits & PtrMask); }
  MSK_CPU_GPU const void *ptr() const { return reinterpret_cast<const void *>(m_bits & PtrMask); }

  template <typename F>
  MSK_CPU_GPU inline auto dispatch(F func) {
    DCHECK(ptr() != nullptr);
    constexpr int n = max_tag();
    return detail::DispatchSplit<n>()(func, *this, tag(), Types());
  }

  template <typename F>
  MSK_CPU_GPU inline auto dispatch(F func) const {
    DCHECK(ptr() != nullptr);
    constexpr int n = max_tag();
    return detail::DispatchSplit<n>()(func, *this, tag(), Types());
  }

  template <typename F>
  MSK_CPU_GPU inline auto dispatch_ref(F func) -> auto && {
    DCHECK(ptr() != nullptr);
    constexpr int n = max_tag();
    return detail::DispatchSplitCRef<n>()(func, *this, tag(), Types());
  }

  template <typename F>
  MSK_CPU_GPU inline auto dispatch_ref(F func) const -> auto && {
    DCHECK(ptr() != nullptr);
    constexpr int n = max_tag();
    return detail::DispatchSplitCRef<n>()(func, *this, tag(), Types());
  }

  template <typename F>
  inline auto dispatch_cpu(F func) {
    DCHECK(ptr() != nullptr);
    constexpr int n = max_tag();
    return detail::DispatchSplitCPU<n>()(func, *this, tag(), Types());
  }

  template <typename F>
  inline auto dispatch_cpu(F func) const {
    DCHECK(ptr() != nullptr);
    constexpr int n = max_tag();
    return detail::DispatchSplitCPU<n>()(func, *this, tag(), Types());
  }

 private:
  static_assert(sizeof(uintptr_t) == 8, "Expected uintptr_t to be 64 bits");

  static constexpr int TagShift = 48;
  static constexpr uint64_t TagMask = (((1ull << 16) - 1) << TagShift);
  static constexpr uint64_t PtrMask = ~TagMask;

  uintptr_t m_bits = 0;
};

}  // namespace misaki::memory