#pragma once

#include "vec4.hpp"

namespace misaki::math {

// Row major matrix
template <typename Value>
struct TMatrix4 {
  using Row = TVector4<Value>;
  using Column = TVector4<Value>;
  using Self = TMatrix4<Value>;

  Row data[4];

  MSK_CPU_GPU TMatrix4() noexcept : TMatrix4(Self::identity()) {
  }

  MSK_CPU_GPU TMatrix4(const Row &r0, const Row &r1, const Row &r2, const Row &r3) noexcept : data{r0, r1, r2, r3} {}

  MSK_CPU_GPU TMatrix4(Value m00, Value m01, Value m02, Value m03,
                       Value m10, Value m11, Value m12, Value m13,
                       Value m20, Value m21, Value m22, Value m23,
                       Value m30, Value m31, Value m32, Value m33) noexcept : data{{m00, m01, m02, m03},
                                                                                   {m10, m11, m12, m13},
                                                                                   {m20, m21, m22, m23},
                                                                                   {m30, m31, m32, m33}} {}

  MSK_CPU_GPU static Self from_rows(const Row &r0, const Row &r1, const Row &r2, const Row &r3) noexcept {
    return Self(r0, r1, r2, r3);
  }

  MSK_CPU_GPU static Self from_cols(const Column &c0, const Column &c1, const Column &c2, const Column &c3) noexcept {
    return Self(c0.x, c1.x, c2.x, c3.x,
                c0.y, c1.y, c2.y, c3.y,
                c0.z, c1.z, c2.z, c3.z,
                c0.w, c1.w, c2.w, c3.w);
  }

  MSK_CPU_GPU static Self constant(Value val) noexcept {
    return Self(val, val, val, val,
                val, val, val, val,
                val, val, val, val,
                val, val, val, val);
  }

  MSK_CPU_GPU static Self diag(const Row &vec) noexcept {
    return Self(vec.x, 0, 0, 0,
                0, vec.y, 0, 0,
                0, 0, vec.z, 0,
                0, 0, 0, vec.w);
  }

  MSK_CPU_GPU static const Self &zeros() noexcept {
    static const Self ret = Self::constant(0);
    return ret;
  }

  MSK_CPU_GPU static const Self &ones() noexcept {
    static const Self ret = Self::constant(0);
    return ret;
  }

  MSK_CPU_GPU static const Self &identity() noexcept {
    static const Self ret = Self::diag(1);
    return ret;
  }
  // Component access
  MSK_CPU_GPU Row &operator[](size_t idx) noexcept {
    return data[idx];
  }

  MSK_CPU_GPU const Row &operator[](size_t idx) const noexcept {
    return data[idx];
  }

  MSK_CPU_GPU Value &operator()(size_t row, size_t col) noexcept {
    return data[row][col];
  }

  MSK_CPU_GPU const Value &operator()(size_t row, size_t col) const noexcept {
    return data[row][col];
  }

  MSK_CPU_GPU Column col(size_t idx) const noexcept {
    return Column(data[0][idx], data[1][idx], data[2][idx], data[3][idx]);
  }

  MSK_CPU_GPU const Row &row(size_t idx) const noexcept {
    return data[idx];
  }

  // Element-wise computation
  MSK_CPU_GPU Self &operator+=(const Self &rhs) noexcept { return *this = *this + rhs; }
  MSK_CPU_GPU Self &operator-=(const Self &rhs) noexcept { return *this = *this - rhs; }
  MSK_CPU_GPU Self &operator*=(const Self &rhs) noexcept { return *this = *this * rhs; }

  MSK_CPU_GPU Self &operator+=(Value rhs) noexcept { return *this = *this + rhs; }

  MSK_CPU_GPU Self &operator-=(Value rhs) noexcept { return *this = *this - rhs; }

  MSK_CPU_GPU Self &operator*=(Value rhs) noexcept { return *this = *this * rhs; }

  MSK_CPU_GPU Self &operator/=(Value rhs) noexcept { return *this = *this / rhs; }

  // Math operation
  MSK_CPU_GPU decltype(auto) determinant() const noexcept {
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
    Value coef00 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
    Value coef02 = data[1][2] * data[3][3] - data[3][2] * data[1][3];
    Value coef03 = data[1][2] * data[2][3] - data[2][2] * data[1][3];

    Value coef04 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    Value coef06 = data[1][1] * data[3][3] - data[3][1] * data[1][3];
    Value coef07 = data[1][1] * data[2][3] - data[2][1] * data[1][3];

    Value coef08 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    Value coef10 = data[1][1] * data[3][2] - data[3][1] * data[1][2];
    Value coef11 = data[1][1] * data[2][2] - data[2][1] * data[1][2];

    Value coef12 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    Value coef14 = data[1][0] * data[3][3] - data[3][0] * data[1][3];
    Value coef15 = data[1][0] * data[2][3] - data[2][0] * data[1][3];

    Value coef16 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    Value coef18 = data[1][0] * data[3][2] - data[3][0] * data[1][2];
    Value coef19 = data[1][0] * data[2][2] - data[2][0] * data[1][2];

    Value coef20 = data[2][0] * data[3][1] - data[3][0] * data[2][1];
    Value coef22 = data[1][0] * data[3][1] - data[3][0] * data[1][1];
    Value coef23 = data[1][0] * data[2][1] - data[2][0] * data[1][1];

    using Vector = TVector4<Value>;

    Vector cofac0(coef00, coef00, coef02, coef03);
    Vector cofac1(coef04, coef04, coef06, coef07);
    Vector cofac2(coef08, coef08, coef10, coef11);
    Vector cofac3(coef12, coef12, coef14, coef15);
    Vector cofac4(coef16, coef16, coef18, coef19);
    Vector cofac5(coef20, coef20, coef22, coef23);

    Vector vec0(data[1][0], data[0][0], data[0][0], data[0][0]);
    Vector vec1(data[1][1], data[0][1], data[0][1], data[0][1]);
    Vector vec2(data[1][2], data[0][2], data[0][2], data[0][2]);
    Vector vec3(data[1][3], data[0][3], data[0][3], data[0][3]);

    Vector inv0(vec1 * cofac0 - vec2 * cofac1 + vec3 * cofac2);
    Vector inv1(vec0 * cofac0 - vec2 * cofac3 + vec3 * cofac4);
    Vector inv2(vec0 * cofac1 - vec1 * cofac3 + vec3 * cofac5);
    Vector inv3(vec0 * cofac2 - vec1 * cofac4 + vec2 * cofac5);

    Vector sign_a(+1, -1, +1, -1);
    Vector sign_b(-1, +1, -1, +1);
    Self inversed(inv0 * sign_a, inv1 * sign_b, inv2 * sign_a,
                  inv3 * sign_b);

    Vector row0(inversed[0][0], inversed[1][0], inversed[2][0], inversed[3][0]);

    Vector dot0(data[0] * row0);
    Value dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
    return dot1;
  }

  MSK_CPU_GPU mstd::optional<Self> inverse() const noexcept {
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
    Value coef00 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
    Value coef02 = data[1][2] * data[3][3] - data[3][2] * data[1][3];
    Value coef03 = data[1][2] * data[2][3] - data[2][2] * data[1][3];

    Value coef04 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    Value coef06 = data[1][1] * data[3][3] - data[3][1] * data[1][3];
    Value coef07 = data[1][1] * data[2][3] - data[2][1] * data[1][3];

    Value coef08 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    Value coef10 = data[1][1] * data[3][2] - data[3][1] * data[1][2];
    Value coef11 = data[1][1] * data[2][2] - data[2][1] * data[1][2];

    Value coef12 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    Value coef14 = data[1][0] * data[3][3] - data[3][0] * data[1][3];
    Value coef15 = data[1][0] * data[2][3] - data[2][0] * data[1][3];

    Value coef16 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    Value coef18 = data[1][0] * data[3][2] - data[3][0] * data[1][2];
    Value coef19 = data[1][0] * data[2][2] - data[2][0] * data[1][2];

    Value coef20 = data[2][0] * data[3][1] - data[3][0] * data[2][1];
    Value coef22 = data[1][0] * data[3][1] - data[3][0] * data[1][1];
    Value coef23 = data[1][0] * data[2][1] - data[2][0] * data[1][1];

    using Vector = TVector4<Value>;

    Vector cofac0(coef00, coef00, coef02, coef03);
    Vector cofac1(coef04, coef04, coef06, coef07);
    Vector cofac2(coef08, coef08, coef10, coef11);
    Vector cofac3(coef12, coef12, coef14, coef15);
    Vector cofac4(coef16, coef16, coef18, coef19);
    Vector cofac5(coef20, coef20, coef22, coef23);

    Vector vec0(data[1][0], data[0][0], data[0][0], data[0][0]);
    Vector vec1(data[1][1], data[0][1], data[0][1], data[0][1]);
    Vector vec2(data[1][2], data[0][2], data[0][2], data[0][2]);
    Vector vec3(data[1][3], data[0][3], data[0][3], data[0][3]);

    Vector inv0(vec1 * cofac0 - vec2 * cofac1 + vec3 * cofac2);
    Vector inv1(vec0 * cofac0 - vec2 * cofac3 + vec3 * cofac4);
    Vector inv2(vec0 * cofac1 - vec1 * cofac3 + vec3 * cofac5);
    Vector inv3(vec0 * cofac2 - vec1 * cofac4 + vec2 * cofac5);

    Vector sign_a(+1, -1, +1, -1);
    Vector sign_b(-1, +1, -1, +1);
    Self inversed(inv0 * sign_a, inv1 * sign_b, inv2 * sign_a,
                  inv3 * sign_b);

    Vector row0(inversed[0][0], inversed[1][0], inversed[2][0], inversed[3][0]);

    Vector dot0(data[0] * row0);
    Value dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
    if (dot1 == 0) return {};
    Value one_over_det = static_cast<Value>(1) / dot1;
    return inversed * one_over_det;
  }

  MSK_CPU_GPU Self transpose() const noexcept {
    return Self::from_cols(data[0], data[1], data[2], data[3]);
  }

  MSK_CPU_GPU bool operator==(const Self &m) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        if (data[i][j] != m.data[i][j])
          return false;
    return true;
  }

  MSK_CPU_GPU bool operator!=(const Self &m) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        if (data[i][j] != m.data[i][j])
          return true;
    return false;
  }

  MSK_CPU_GPU bool operator<(const Self &m) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j) {
        if (data[i][j] < m.data[i][j])
          return true;
        if (data[i][j] > m.data[i][j])
          return false;
      }
    return false;
  }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator+(const TMatrix4<Value> &lhs, const TMatrix4<Value> &rhs) noexcept {
  return TMatrix4<Value>::from_rows(lhs[0] + rhs[0], lhs[1] + rhs[1],
                                    lhs[2] + rhs[2], lhs[3] + rhs[3]);
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator-(const TMatrix4<Value> &lhs, const TMatrix4<Value> &rhs) noexcept {
  return TMatrix4<Value>::from_rows(lhs[0] - rhs[0], lhs[1] - rhs[1],
                                    lhs[2] - rhs[2], lhs[3] - rhs[3]);
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator*(const TMatrix4<Value> &lhs, const TMatrix4<Value> &rhs) noexcept {
  TMatrix4<Value> ret;
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      ret(r, c) = dot(lhs.row(r), rhs.col(c));
    }
  }
  return ret;
}

template <typename Value>
MSK_CPU_GPU TVector4<Value> operator*(const TMatrix4<Value> &lhs, const TVector4<Value> &rhs) noexcept {
  TVector4<Value> ret;
  for (int i = 0; i < 4; ++i) {
    ret[i] = dot(lhs.row(i), rhs);
  }
  return ret;
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator+(const TMatrix4<Value> &lhs, Value rhs) noexcept {
  return TMatrix4<Value>(lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs, lhs[3] + rhs);
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator-(const TMatrix4<Value> &lhs, Value rhs) noexcept {
  return TMatrix4<Value>(lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs, lhs[3] - rhs);
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator*(const TMatrix4<Value> &lhs, Value rhs) noexcept {
  return TMatrix4<Value>(lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs, lhs[3] * rhs);
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator/(const TMatrix4<Value> &lhs, Value rhs) noexcept {
  return TMatrix4<Value>(lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs, lhs[3] / rhs);
}

template <typename Value>
MSK_CPU_GPU TMatrix4<Value> operator*(Value lhs, const TMatrix4<Value> &rhs) noexcept {
  return rhs * lhs;
}

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TMatrix4<Value> &mat) {
  oss << "[";
  oss << mat.row(0) << ",\n ";
  oss << mat.row(1) << ",\n ";
  oss << mat.row(2) << ",\n ";
  oss << mat.row(3);
  oss << "]";
  return oss;
}

// Type alias
using Matrix4f = TMatrix4<float>;
using Matrix4d = TMatrix4<double>;

}  // namespace misaki::math