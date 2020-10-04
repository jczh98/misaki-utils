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

  template <typename... Columns>
  MSK_XPU static Matrix from_cols(const Columns &... cols) {
    return Matrix(cols...);
  }

  template <typename... Rows>
  MSK_XPU static Matrix from_rows(const Rows &... rows) {
    return transpose(Matrix(rows...));
  }

  MSK_XPU Column &col(size_t index) { return coeff(index); }
  MSK_XPU const Column &col(size_t index) const { return coeff(index); }

  MSK_XPU Row row(size_t index) const {
    Row r;
    for (size_t i = 0; i < Size_; ++i) r.coeff(i) = coeff(i).coeff(index);
    return r;
  }

  MSK_XPU decltype(auto) operator()(size_t i, size_t j) { return coeff(j, i); }

  MSK_XPU decltype(auto) operator()(size_t i, size_t j) const {
    return coeff(j, i);
  }
};

template <typename T0, typename T1, size_t Size,
          typename Result = Matrix<std::common_type_t<T0, T1>, Size>,
          typename Column = Array<T0, Size>>
MSK_XPU Result operator*(const Matrix<T0, Size> &m0,
                         const Matrix<T1, Size> &m1) {
  Result result;
  for (int r = 0; r < Size; ++r) {
    for (int c = 0; c < Size; ++c) {
      result(r, c) = dot(m0.row(r), m1.col(c));
    }
  }
  return result;
}

template <typename T0, typename T1, size_t Size,
          std::enable_if_t<std::is_floating_point_v<T1>, int> = 0>
MSK_XPU auto operator*(const Matrix<T0, Size> &m, const T1 &s) {
  using EValue = std::common_type_t<T0, T1>;
  using EMatrix = Matrix<EValue, Size>;
  EMatrix result;
  for (int i = 0; i < Size; ++i)
    for (int j = 0; j < Size; ++j) result.coeff(i, j) = m.coeff(i, j) * s;
  return result;
}

template <typename T0, typename T1, size_t Size,
          std::enable_if_t<std::is_floating_point_v<T1>, int> = 0>
MSK_XPU auto operator*(const Matrix<T0, Size> &m, const Array<T1, Size> &s) {
  using EValue = std::common_type_t<T0, T1>;
  using EVector = Array<EValue, Size>;
  EVector ret;
  for (int i = 0; i < Size; ++i) {
    ret.coeff(i) = dot(m.row(i), s);
  }
  return ret;
}

template <typename Value, size_t Size>
MSK_XPU auto transpose(const Matrix<Value, Size> &m) {
  Matrix<Value, Size> r;
  for (size_t i = 0; i < Size; ++i)
    for (size_t j = 0; j < Size; ++j) r.coeff(i, j) = m.coeff(j, i);
  return r;
}

template <typename Value>
MSK_XPU Matrix<Value, 2> transpose(const Matrix<Value, 2> &m) {
  return Matrix<Value, 2>(m(0, 0), m(1, 0), m(0, 1), m(1, 1));
}

template <typename Value>
MSK_XPU auto determinant(const Matrix<Value, 2> &m) {
  return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
}

template <typename Value>
MSK_XPU Matrix<Value, 2> inverse(const Matrix<Value, 2> &m) {
  Value inv_det = 1.f / (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0));
  return Matrix<Value, 2>(m(1, 1) * inv_det, -m(0, 1) * inv_det,
                          -m(1, 0) * inv_det, m(0, 0) * inv_det);
}

template <typename Value>
MSK_XPU auto determinant(const Matrix<Value, 3> &m) {
  return m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
         m(1, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) +
         m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2));
}

template <typename Value>
MSK_XPU Matrix<Value, 3> inverse(const Matrix<Value, 3> &m) {
  Value det = determinant(m);
  return Value(1.0) / det *
         Matrix<Value, 3>(m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2),
                          m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2),
                          m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2),
                          m(2, 0) * m(1, 2) - m(1, 0) * m(2, 2),
                          m(0, 0) * m(2, 2) - m(2, 0) * m(0, 2),
                          m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2),
                          m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1),
                          m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1),
                          m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1));
}

template <typename Value>
MSK_XPU auto determinant(const Matrix<Value, 4> &m) {
  // This function is adopted from GLM
  /*
  ================================================================================
  OpenGL Mathematics (GLM)
  --------------------------------------------------------------------------------
  GLM is licensed under The Happy Bunny License and MIT License
  ================================================================================
  The Happy Bunny License (Modified MIT License)
  --------------------------------------------------------------------------------
  Copyright (c) 2005 - 2014 G-Truc Creation
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  Restrictions:
  By making use of the Software for military purposes, you choose to make a
  Bunny unhappy.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ================================================================================
  The MIT License
  --------------------------------------------------------------------------------
  Copyright (c) 2005 - 2014 G-Truc Creation
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  */
  Value coef00 = m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3);
  Value coef02 = m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3);
  Value coef03 = m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3);
  Value coef04 = m(2, 1) * m(3, 3) - m(3, 1) * m(2, 3);
  Value coef06 = m(1, 1) * m(3, 3) - m(3, 1) * m(1, 3);
  Value coef07 = m(1, 1) * m(2, 3) - m(2, 1) * m(1, 3);
  Value coef08 = m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2);
  Value coef10 = m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2);
  Value coef11 = m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2);
  Value coef12 = m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3);
  Value coef14 = m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3);
  Value coef15 = m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3);
  Value coef16 = m(2, 0) * m(3, 2) - m(3, 0) * m(2, 2);
  Value coef18 = m(1, 0) * m(3, 2) - m(3, 0) * m(1, 2);
  Value coef19 = m(1, 0) * m(2, 2) - m(2, 0) * m(1, 2);
  Value coef20 = m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1);
  Value coef22 = m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1);
  Value coef23 = m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1);

  using Vector = Array<Value, 4>;

  Vector cofac0(coef00, coef00, coef02, coef03);
  Vector cofac1(coef04, coef04, coef06, coef07);
  Vector cofac2(coef08, coef08, coef10, coef11);
  Vector cofac3(coef12, coef12, coef14, coef15);
  Vector cofac4(coef16, coef16, coef18, coef19);
  Vector cofac5(coef20, coef20, coef22, coef23);

  Vector vec0(m(1, 0), m(0, 0), m(0, 0), m(0, 0));
  Vector vec1(m(1, 1), m(0, 1), m(0, 1), m(0, 1));
  Vector vec2(m(1, 2), m(0, 2), m(0, 2), m(0, 2));
  Vector vec3(m(1, 3), m(0, 3), m(0, 3), m(0, 3));

  Vector inv0(vec1 * cofac0 - vec2 * cofac1 + vec3 * cofac2);
  Vector inv1(vec0 * cofac0 - vec2 * cofac3 + vec3 * cofac4);
  Vector inv2(vec0 * cofac1 - vec1 * cofac3 + vec3 * cofac5);
  Vector inv3(vec0 * cofac2 - vec1 * cofac4 + vec2 * cofac5);

  Vector sign_a(+1, -1, +1, -1);
  Vector sign_b(-1, +1, -1, +1);
  Matrix<Value, 4> inversed = Matrix<Value, 4>::from_rows(
      inv0 * sign_a, inv1 * sign_b, inv2 * sign_a, inv3 * sign_b);

  Vector row0(inversed(0, 0), inversed(1, 0), inversed(2, 0), inversed(3, 0));

  Vector dot0(m.row(0) * row0);
  Value dot1 = (dot0.x() + dot0.y()) + (dot0.z() + dot0.w());
  return dot1;
}

template <typename Value>
MSK_XPU Matrix<Value, 4> inverse(const Matrix<Value, 4> &m) {
  // This function is adopted from GLM
  /*
  ================================================================================
  OpenGL Mathematics (GLM)
  --------------------------------------------------------------------------------
  GLM is licensed under The Happy Bunny License and MIT License
  ================================================================================
  The Happy Bunny License (Modified MIT License)
  --------------------------------------------------------------------------------
  Copyright (c) 2005 - 2014 G-Truc Creation
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  Restrictions:
  By making use of the Software for military purposes, you choose to make a
  Bunny unhappy.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ================================================================================
  The MIT License
  --------------------------------------------------------------------------------
  Copyright (c) 2005 - 2014 G-Truc Creation
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  */
  Value coef00 = m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3);
  Value coef02 = m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3);
  Value coef03 = m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3);
  Value coef04 = m(2, 1) * m(3, 3) - m(3, 1) * m(2, 3);
  Value coef06 = m(1, 1) * m(3, 3) - m(3, 1) * m(1, 3);
  Value coef07 = m(1, 1) * m(2, 3) - m(2, 1) * m(1, 3);
  Value coef08 = m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2);
  Value coef10 = m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2);
  Value coef11 = m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2);
  Value coef12 = m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3);
  Value coef14 = m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3);
  Value coef15 = m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3);
  Value coef16 = m(2, 0) * m(3, 2) - m(3, 0) * m(2, 2);
  Value coef18 = m(1, 0) * m(3, 2) - m(3, 0) * m(1, 2);
  Value coef19 = m(1, 0) * m(2, 2) - m(2, 0) * m(1, 2);
  Value coef20 = m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1);
  Value coef22 = m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1);
  Value coef23 = m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1);

  using Vector = Array<Value, 4>;

  Vector cofac0(coef00, coef00, coef02, coef03);
  Vector cofac1(coef04, coef04, coef06, coef07);
  Vector cofac2(coef08, coef08, coef10, coef11);
  Vector cofac3(coef12, coef12, coef14, coef15);
  Vector cofac4(coef16, coef16, coef18, coef19);
  Vector cofac5(coef20, coef20, coef22, coef23);

  Vector vec0(m(1, 0), m(0, 0), m(0, 0), m(0, 0));
  Vector vec1(m(1, 1), m(0, 1), m(0, 1), m(0, 1));
  Vector vec2(m(1, 2), m(0, 2), m(0, 2), m(0, 2));
  Vector vec3(m(1, 3), m(0, 3), m(0, 3), m(0, 3));

  Vector inv0(vec1 * cofac0 - vec2 * cofac1 + vec3 * cofac2);
  Vector inv1(vec0 * cofac0 - vec2 * cofac3 + vec3 * cofac4);
  Vector inv2(vec0 * cofac1 - vec1 * cofac3 + vec3 * cofac5);
  Vector inv3(vec0 * cofac2 - vec1 * cofac4 + vec2 * cofac5);

  Vector sign_a(+1, -1, +1, -1);
  Vector sign_b(-1, +1, -1, +1);
  Matrix<Value, 4> inversed = Matrix<Value, 4>::from_rows(
      inv0 * sign_a, inv1 * sign_b, inv2 * sign_a, inv3 * sign_b);

  Vector row0(inversed(0, 0), inversed(1, 0), inversed(2, 0), inversed(3, 0));

  Vector dot0(m.row(0) * row0);
  Value dot1 = (dot0.x() + dot0.y()) + (dot0.z() + dot0.w());
  Value one_over_det = static_cast<Value>(1) / dot1;
  return inversed * one_over_det;
}

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