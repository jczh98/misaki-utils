#pragma once

namespace misaki {
namespace container {

// TypePack Definition
template <typename... Ts>
struct TypePack {
  static constexpr size_t count = sizeof...(Ts);
};

// TypePack Operations
template <typename F, typename... Ts>
void ForEachType(F func, TypePack<Ts...>);

template <typename F>
void ForEachType(F func, TypePack<>) {}

template <typename F, typename T, typename... Ts>
void ForEachType(F func, TypePack<T, Ts...>) {
  func.template operator()<T>();
  ForEachType(func, TypePack<Ts...>());
}

template <typename T>
struct MaxSize;
template <typename T>
struct MaxSize<TypePack<T>> {
  static constexpr size_t size = sizeof(T);
};
template <typename T, typename... Ts>
struct MaxSize<TypePack<T, Ts...>> {
  static constexpr size_t size = (sizeof(T) > MaxSize<TypePack<Ts...>>::size)
                                     ? sizeof(T)
                                     : MaxSize<TypePack<Ts...>>::size;
};

template <typename... Ts>
struct Prepend;
template <typename T, typename... Ts>
struct Prepend<T, TypePack<Ts...>> {
  using type = TypePack<T, Ts...>;
};

template <typename T>
struct RemoveFirst {};
template <typename T, typename... Ts>
struct RemoveFirst<TypePack<T, Ts...>> {
  using type = TypePack<Ts...>;
};

template <typename T>
struct GetFirst {};
template <typename T, typename... Ts>
struct GetFirst<TypePack<T, Ts...>> {
  using type = T;
};

template <int index, typename T, typename... Ts>
struct RemoveFirstN;
template <int index, typename T, typename... Ts>
struct RemoveFirstN<index, TypePack<T, Ts...>> {
  using type = typename RemoveFirstN<index - 1, TypePack<Ts...>>::type;
};
template <typename T, typename... Ts>
struct RemoveFirstN<0, TypePack<T, Ts...>> {
  using type = TypePack<T, Ts...>;
};

template <int index, typename T, typename... Ts>
struct TakeFirstN;
template <int index, typename T, typename... Ts>
struct TakeFirstN<index, TypePack<T, Ts...>> {
  using type =
      typename Prepend<T, typename TakeFirstN<index - 1, TypePack<Ts...>>::type>::type;
};
template <typename T, typename... Ts>
struct TakeFirstN<1, TypePack<T, Ts...>> {
  using type = TypePack<T>;
};

template <typename T, typename... Ts>
struct HasType {};
template <typename T>
struct HasType<T, TypePack<void>> {
  static constexpr bool value = false;
};
template <typename T, typename Tfirst, typename... Ts>
struct HasType<T, TypePack<Tfirst, Ts...>> {
  static constexpr bool value =
      (std::is_same<T, Tfirst>::value || HasType<T, TypePack<Ts...>>::value);
};

}  // namespace container
}  // namespace misaki