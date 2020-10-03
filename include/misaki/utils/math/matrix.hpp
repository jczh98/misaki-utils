#pragma once

#include "array.hpp"

namespace misaki::math {

template <typename Value_, size_t Size_>
class Matrix : public StaticArrayBase<Array<Value_, Size_>, Size_,
                                      Matrix<Value_, Size_>> {
 public:
  using Entry = Value_;
  using Column = Array<Entry, Size_>;
  using Row = Array<Entry, Size_>;

  using Base = StaticArrayBase<Column, Size_, Matrix<Value_, Size_>>;
  using Base::coeff;

  MSK_ARRAY_IMPORT_BASIC(Base, Matrix);
  using Base::operator=;

  Matrix() = default;
  template <typename T>
  MSK_XPU Matrix(T &&v) {
    for (size_t i = 0; i < Size; ++i) {
      coeff(i) = Column(0);
      coeff(i, i) = v;
    }
  }

  // Initialize the matrix from a list of columns
  template <typename... Args,
            std::enable_if_t<
                sizeof...(Args) == Size_ &&
                    std::conjunction_v<std::is_constructible<Column, Args>...>,
                int> = 0>
  MSK_XPU Matrix(const Args &... args) : Base(args...) {}

  // Initialize the matrix from a list of entries in row-major order
  template <typename... Args,
            std::enable_if_t<
                (sizeof...(Args) == Size_ * Size_ &&
                 std::conjunction_v<std::is_constructible<Entry, Args>...>),
                int> = 0>
  MSK_XPU Matrix(const Args &... args) {
    alignas(alignof(Column)) Entry values[sizeof...(Args)] = {Entry(args)...};
    for (size_t j = 0; j < Size; ++j)
      for (size_t i = 0; i < Size; ++i) coeff(j, i) = values[i * Size + j];
  }

  MSK_XPU Column &col(size_t index) { return coeff(index); }
  MSK_XPU const Column &col(size_t index) const { return coeff(index); }

  MSK_XPU Row row(size_t index) const {
    Row r;
    for (size_t i = 0; i < Size_; ++i) r.coeff(i) = coeff(i).coeff(index);
    return r;
  }
};

template <typename Value, size_t Size>
std::ostream &operator<<(std::ostream &oss, const Matrix<Value, Size> &m) {
  oss << "[";
  if constexpr (Size == 2) {
    oss << m.row(0) << ",\n " << m.row(1);
  } else if constexpr (Size == 1) {
    oss << m.row(0);
  } else {
    oss << m.row(0) << ",\n";
    for (int i = 1; i < Size - 1; ++i) oss << " " << m.row(i) << ",\n";
    oss << " " << m.row(Size - 1);
  }

  oss << "]";
  return oss;
}

}  // namespace misaki::math