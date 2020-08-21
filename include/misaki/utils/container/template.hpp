#pragma once

namespace misaki {
namespace container {

// TypePack Definition
template <typename... Ts>
struct TypePack {
  static constexpr size_t count = sizeof...(Ts);
};

}  // namespace container
}  // namespace misaki