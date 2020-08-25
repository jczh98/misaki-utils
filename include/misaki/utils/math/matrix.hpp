#pragma once

#include "../stl.hpp"
#include "../system.h"
#include "common.hpp"

namespace misaki::math {

namespace {

template <typename T, int N>
MSK_CPU_GPU inline void init(T m[N][N], int i, int j) {}

template <typename T, int N, typename... Args>
MSK_CPU_GPU inline void init(T m[N][N], int i, int j, T v, Args... args) {
  m[i][j] = v;
  if (++j == N) {
    ++i;
    j = 0;
  }
  init<T, N>(m, i, j, args...);
}

template <typename T, int N>
MSK_CPU_GPU inline void init_diagonal(T m[N][N], int i) {}

template <typename T, int N, typename... Args>
MSK_CPU_GPU inline void init_diagonal(T m[N][N], int i, T v, Args... args) {
  m[i][i] = v;
  initDiag<T, N>(m, i + 1, args...);
}

}  // namespace

template <typename T, int N>
class Matrix {
 public:
  MSK_CPU_GPU Matrix() {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        m[i][j] = 0;
  }

  MSK_CPU_GPU Matrix(const T mat[N][N]) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        m[i][j] = mat[i][j];
  }

  MSK_CPU_GPU Matrix(mstd::span<const T> t) {
    CHECK_EQ(N * N, t.size());
    for (int i = 0; i < N * N; ++i)
      m[i / N][i % N] = t[i];
  }

  template <typename... Args>
  MSK_CPU_GPU Matrix(T v, Args... args) {
    static_assert(1 + sizeof...(Args) == N * N,
                  "Incorrect number of values provided to Matrix constructor");
    init<T, N>(m, 0, 0, v, args...);
  }

  template <typename... Args>
  MSK_CPU_GPU static Matrix diagonal(T v, Args... args) {
    static_assert(1 + sizeof...(Args) == N,
                  "Incorrect number of values provided to Matrix::diagonal");
    Matrix m;
    init_diagonal<T, N>(m.m, 0, v, args...);
    return m;
  }

  MSK_CPU_GPU
  bool operator==(const Matrix<T, N> &m2) const {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        if (m[i][j] != m2.m[i][j])
          return false;
    return true;
  }

  MSK_CPU_GPU
  bool operator!=(const Matrix<T, N> &m2) const {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        if (m[i][j] != m2.m[i][j])
          return true;
    return false;
  }

  MSK_CPU_GPU
  bool operator<(const Matrix<T, N> &m2) const {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j) {
        if (m[i][j] < m2.m[i][j])
          return true;
        if (m[i][j] > m2.m[i][j])
          return false;
      }
    return false;
  }

  MSK_CPU_GPU
  mstd::span<const T> operator[](int i) const { return m[i]; }

  MSK_CPU_GPU
  mstd::span<T> operator[](int i) { return mstd::span<T>(m[i]); }

  MSK_CPU_GPU T determinant() const;

  MSK_CPU_GPU mstd::optional<Matrix<T, N>> inverse();

  MSK_CPU_GPU Matrix<T, N> transpose() {
    Matrix<T, N> r;
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        r[i][j] = m[j][i];
    return r;
  }

  MSK_CPU_GPU
  bool is_identity() const {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j) {
        if (i == j) {
          if (m[i][j] != 1)
            return false;
        } else if (m[i][j] != 0)
          return false;
      }
    return true;
  }

 private:
  T m[N][N];
};

// Operator functions

// Only for vector or span
template <typename Tresult, int N, typename Float, typename T>
MSK_CPU_GPU inline Tresult Mul(const Matrix<Float, N> &m, const T &v) {
  Tresult result;
  for (int i = 0; i < N; ++i) {
    result[i] = 0;
    for (int j = 0; j < N; ++j)
      result[i] += m[i][j] * v[j];
  }
  return result;
}

template <int N, typename Float, typename T>
MSK_CPU_GPU inline T operator*(const Matrix<Float, N> &m, const T &v) {
  return multiply<T>(m, v);
}

template <typename Float, int N>
MSK_CPU_GPU inline Matrix<Float, N> operator*(const Matrix<Float, N> &m1,
                                              const Matrix<Float, N> &m2) {
  Matrix<Float, N> r;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) {
      r[i][j] = 0;
      for (int k = 0; k < N; ++k)
        r[i][j] = fmadd(m1[i][k], m2[k][j], r[i][j]);
    }
  return r;
}

// Instantiate matrix template determinat
template <>
MSK_CPU_GPU inline float Matrix<float, 1>::determinant() const {
  return m[0][0];
}

template <>
MSK_CPU_GPU inline float Matrix<float, 2>::determinant() const {
  return diff_of_prod(m[0][0], m[1][1], m[0][1], m[1][0]);
}

template <>
MSK_CPU_GPU inline float Matrix<float, 3>::determinant() const {
  float minor12 = diff_of_prod(m[1][1], m[2][2], m[1][2], m[2][1]);
  float minor02 = diff_of_prod(m[1][0], m[2][2], m[1][2], m[2][0]);
  float minor01 = diff_of_prod(m[1][0], m[2][1], m[1][1], m[2][0]);
  return fmadd(m[0][2], minor01,
               diff_of_prod(m[0][0], minor12, m[0][1], minor02));
}

template <>
MSK_CPU_GPU inline float Matrix<float, 4>::determinant() const {
  float s0 = diff_of_prod(m[0][0], m[1][1], m[1][0], m[0][1]);
  float s1 = diff_of_prod(m[0][0], m[1][2], m[1][0], m[0][2]);
  float s2 = diff_of_prod(m[0][0], m[1][3], m[1][0], m[0][3]);

  float s3 = diff_of_prod(m[0][1], m[1][2], m[1][1], m[0][2]);
  float s4 = diff_of_prod(m[0][1], m[1][3], m[1][1], m[0][3]);
  float s5 = diff_of_prod(m[0][2], m[1][3], m[1][2], m[0][3]);

  float c0 = diff_of_prod(m[2][0], m[3][1], m[3][0], m[2][1]);
  float c1 = diff_of_prod(m[2][0], m[3][2], m[3][0], m[2][2]);
  float c2 = diff_of_prod(m[2][0], m[3][3], m[3][0], m[2][3]);

  float c3 = diff_of_prod(m[2][1], m[3][2], m[3][1], m[2][2]);
  float c4 = diff_of_prod(m[2][1], m[3][3], m[3][1], m[2][3]);
  float c5 = diff_of_prod(m[2][2], m[3][3], m[3][2], m[2][3]);

  return (diff_of_prod(s0, c5, s1, c4) + diff_of_prod(s2, c3, -s3, c2) +
          diff_of_prod(s5, c0, s4, c1));
}

template <>
MSK_CPU_GPU inline double Matrix<double, 1>::determinant() const {
  return m[0][0];
}

template <>
MSK_CPU_GPU inline double Matrix<double, 2>::determinant() const {
  return diff_of_prod(m[0][0], m[1][1], m[0][1], m[1][0]);
}

template <>
MSK_CPU_GPU inline double Matrix<double, 3>::determinant() const {
  double minor12 = diff_of_prod(m[1][1], m[2][2], m[1][2], m[2][1]);
  double minor02 = diff_of_prod(m[1][0], m[2][2], m[1][2], m[2][0]);
  double minor01 = diff_of_prod(m[1][0], m[2][1], m[1][1], m[2][0]);
  return fmadd(m[0][2], minor01,
               diff_of_prod(m[0][0], minor12, m[0][1], minor02));
}

template <>
MSK_CPU_GPU inline double Matrix<double, 4>::determinant() const {
  double s0 = diff_of_prod(m[0][0], m[1][1], m[1][0], m[0][1]);
  double s1 = diff_of_prod(m[0][0], m[1][2], m[1][0], m[0][2]);
  double s2 = diff_of_prod(m[0][0], m[1][3], m[1][0], m[0][3]);

  double s3 = diff_of_prod(m[0][1], m[1][2], m[1][1], m[0][2]);
  double s4 = diff_of_prod(m[0][1], m[1][3], m[1][1], m[0][3]);
  double s5 = diff_of_prod(m[0][2], m[1][3], m[1][2], m[0][3]);

  double c0 = diff_of_prod(m[2][0], m[3][1], m[3][0], m[2][1]);
  double c1 = diff_of_prod(m[2][0], m[3][2], m[3][0], m[2][2]);
  double c2 = diff_of_prod(m[2][0], m[3][3], m[3][0], m[2][3]);

  double c3 = diff_of_prod(m[2][1], m[3][2], m[3][1], m[2][2]);
  double c4 = diff_of_prod(m[2][1], m[3][3], m[3][1], m[2][3]);
  double c5 = diff_of_prod(m[2][2], m[3][3], m[3][2], m[2][3]);

  return (diff_of_prod(s0, c5, s1, c4) + diff_of_prod(s2, c3, -s3, c2) +
          diff_of_prod(s5, c0, s4, c1));
}

template <typename Float, int N>
MSK_CPU_GPU inline Float Matrix<Float, N>::determinant() const {
  Matrix<Float, N - 1> sub;
  Float det = 0;
  // Inefficient, but we don't currently use N>4 anyway..
  for (int i = 0; i < N; ++i) {
    // Sub-matrix without row 0 and column i
    for (int j = 0; j < N - 1; ++j)
      for (int k = 0; k < N - 1; ++k)
        sub[j][k] = m[j + 1][k < i ? k : k + 1];

    Float sign = (i & 1) ? -1 : 1;
    det += sign * m[0][i] * sub.determinant();
  }
  return det;
}
// Instantiate matrix template inverse
template <>
MSK_CPU_GPU inline mstd::optional<Matrix<float, 3>> Matrix<float, 3>::inverse() {
  float det = determinant();
  if (det == 0)
    return {};
  float inv_det = 1 / det;

  Matrix<float, 3> r;

  r[0][0] = inv_det * diff_of_prod(m[1][1], m[2][2], m[1][2], m[2][1]);
  r[1][0] = inv_det * diff_of_prod(m[1][2], m[2][0], m[1][0], m[2][2]);
  r[2][0] = inv_det * diff_of_prod(m[1][0], m[2][1], m[1][1], m[2][0]);
  r[0][1] = inv_det * diff_of_prod(m[0][2], m[2][1], m[0][1], m[2][2]);
  r[1][1] = inv_det * diff_of_prod(m[0][0], m[2][2], m[0][2], m[2][0]);
  r[2][1] = inv_det * diff_of_prod(m[0][1], m[2][0], m[0][0], m[2][1]);
  r[0][2] = inv_det * diff_of_prod(m[0][1], m[1][2], m[0][2], m[1][1]);
  r[1][2] = inv_det * diff_of_prod(m[0][2], m[1][0], m[0][0], m[1][2]);
  r[2][2] = inv_det * diff_of_prod(m[0][0], m[1][1], m[0][1], m[1][0]);

  return r;
}

template <>
MSK_CPU_GPU inline mstd::optional<Matrix<float, 4>> Matrix<float, 4>::inverse() {
  // Via: https://github.com/google/ion/blob/master/ion/math/matrixutils.cc,
  // (c) Google, Apache license.

  // For 4x4 do not compute the adjugate as the transpose of the cofactor
  // matrix, because this results in extra work. Several calculations can be
  // shared across the sub-determinants.
  //
  // This approach is explained in David Eberly's Geometric Tools book,
  // excerpted here:
  //   http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
  float s0 = diff_of_prod(m[0][0], m[1][1], m[1][0], m[0][1]);
  float s1 = diff_of_prod(m[0][0], m[1][2], m[1][0], m[0][2]);
  float s2 = diff_of_prod(m[0][0], m[1][3], m[1][0], m[0][3]);

  float s3 = diff_of_prod(m[0][1], m[1][2], m[1][1], m[0][2]);
  float s4 = diff_of_prod(m[0][1], m[1][3], m[1][1], m[0][3]);
  float s5 = diff_of_prod(m[0][2], m[1][3], m[1][2], m[0][3]);

  float c0 = diff_of_prod(m[2][0], m[3][1], m[3][0], m[2][1]);
  float c1 = diff_of_prod(m[2][0], m[3][2], m[3][0], m[2][2]);
  float c2 = diff_of_prod(m[2][0], m[3][3], m[3][0], m[2][3]);

  float c3 = diff_of_prod(m[2][1], m[3][2], m[3][1], m[2][2]);
  float c4 = diff_of_prod(m[2][1], m[3][3], m[3][1], m[2][3]);
  float c5 = diff_of_prod(m[2][2], m[3][3], m[3][2], m[2][3]);

  float determinant = inner_prod(s0, c5, -s1, c4, s2, c3, s3, c2, s5, c0, -s4, c1);
  if (determinant == 0)
    return {};
  float s = 1 / determinant;

  float inv[4][4] = {s * inner_prod(m[1][1], c5, m[1][3], c3, -m[1][2], c4),
                     s * inner_prod(-m[0][1], c5, m[0][2], c4, -m[0][3], c3),
                     s * inner_prod(m[3][1], s5, m[3][3], s3, -m[3][2], s4),
                     s * inner_prod(-m[2][1], s5, m[2][2], s4, -m[2][3], s3),

                     s * inner_prod(-m[1][0], c5, m[1][2], c2, -m[1][3], c1),
                     s * inner_prod(m[0][0], c5, m[0][3], c1, -m[0][2], c2),
                     s * inner_prod(-m[3][0], s5, m[3][2], s2, -m[3][3], s1),
                     s * inner_prod(m[2][0], s5, m[2][3], s1, -m[2][2], s2),

                     s * inner_prod(m[1][0], c4, m[1][3], c0, -m[1][1], c2),
                     s * inner_prod(-m[0][0], c4, m[0][1], c2, -m[0][3], c0),
                     s * inner_prod(m[3][0], s4, m[3][3], s0, -m[3][1], s2),
                     s * inner_prod(-m[2][0], s4, m[2][1], s2, -m[2][3], s0),

                     s * inner_prod(-m[1][0], c3, m[1][1], c1, -m[1][2], c0),
                     s * inner_prod(m[0][0], c3, m[0][2], c0, -m[0][1], c1),
                     s * inner_prod(-m[3][0], s3, m[3][1], s1, -m[3][2], s0),
                     s * inner_prod(m[2][0], s3, m[2][2], s0, -m[2][1], s1)};

  return Matrix<float, 4>(inv);
}

}  // namespace misaki::math