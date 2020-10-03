#pragma once

#include "array.hpp"

namespace misaki::math {

template <typename Value_, size_t Size_>
    class Color : StaticArrayBase < Value_,
    Size_, Color<Value_, Size_> {
 public:
  using Base = StaticArrayBase < Value_, Size_, Color<Value_, Size_>;
  MSK_ARRAY_IMPORT(Base, Color)
};

}  // namespace misaki::math