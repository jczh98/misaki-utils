#pragma once

#include <Eigen/Dense>
#include <type_traits>
#include <utility>

namespace misaki::math {

// ---------------------------Vector-------------------------------

template <typename Scalar, int Dimension>
struct TVector : public Eigen::Matrix<Scalar, Dimension, 1> {
 public:
  using Base = Eigen::Matrix<Scalar, Dimension, 1>;
  using Self = TVector<Scalar, Dimension>;

  TVector() { Base::setConstant(Scalar(0)); }

  template <typename Value, std::enable_if_t<std::is_arithmetic<Value>::value, int> = 0>
  TVector(Value value = (Value)0) { Base::setConstant(Scalar(value)); }

  template <typename... Args, std::enable_if_t<sizeof...(Args) == Dimension, int> = 0>
  TVector(Args &&... args) : Base(std::forward<Args>(args)...){};

  template <typename T>
  TVector(const TVector<T, Dimension> &vec) : Base(vec.cast<Scalar>()){};

  template <typename Derived>
  TVector(const Eigen::MatrixBase<Derived> &p)
      : Base(p) {}

  template <typename Derived>
  TVector &operator=(const Eigen::MatrixBase<Derived> &p) {
    this->Base::operator=(p);
    return *this;
  }

  std::string to_string() const {
    std::string result;
    for (size_t i = 0; i < Dimension; ++i) {
      result += std::to_string(this->coeff(i));
      result += i + 1 < Dimension ? ", " : "";
    }
    return "[" + result + "]";
  }
};

template <typename Scalar, int Dimension>
std::ostream &operator<<(
    std::ostream &out, const TVector<Scalar, Dimension> &vec) {
  out << vec.to_string();
  return out;
}

// Unary operators
template <typename T, int D>
TVector<T, D> operator-(const TVector<T, D> &vec) noexcept {
  TVector<T, D>::Base vec_ = vec;
  return TVector<T, D>(vec_);
}

// Binary operators for same type
template <typename T, int D>
TVector<T, D> operator+(const TVector<T, D> &lhs, const TVector<T, D> &rhs) noexcept {
  TVector<T, D>::Base lhs_ = lhs, rhs_ = rhs;
  return TVector<T, D>(lhs_ + rhs_);
}

template <typename T, int D>
TVector<T, D> operator-(const TVector<T, D> &lhs, const TVector<T, D> &rhs) noexcept {
  TVector<T, D>::Base lhs_ = lhs, rhs_ = rhs;
  return TVector<T, D>(lhs_ - rhs_);
}

template <typename T, int D>
TVector<T, D> operator*(const TVector<T, D> &lhs, const TVector<T, D> &rhs) noexcept {
  return TVector<T, D>(lhs.cwiseProduct(rhs));
}

template <typename T, int D>
TVector<T, D> operator/(const TVector<T, D> &lhs, const TVector<T, D> &rhs) noexcept {
  return TVector<T, D>((lhs.array() / rhs.array()).matrix());
}

// Binary operators for different type
template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator+(const TVector<T1, D> &lhs, T2 rhs) noexcept {
  return lhs + TVector<T1, D>(rhs);
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator-(const TVector<T1, D> &lhs, T2 rhs) noexcept {
  return lhs - TVector<T1, D>(rhs);
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator*(const TVector<T1, D> &lhs, T2 rhs) noexcept {
  return lhs * TVector<T1, D>(rhs);
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator/(const TVector<T1, D> &lhs, T2 rhs) noexcept {
  return lhs / TVector<T1, D>(rhs);
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator+(T2 lhs, const TVector<T1, D> &rhs) {
  return TVector<T1, D>(lhs) + rhs;
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator-(T2 lhs, const TVector<T1, D> &rhs) {
  return TVector<T1, D>(lhs) - rhs;
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator*(T2 lhs, const TVector<T1, D> &rhs) {
  return TVector<T1, D>(lhs) * rhs;
}

template <typename T1, typename T2, int D, std::enable_if_t<std::is_arithmetic<T2>::value, int> = 0>
TVector<T1, D> operator/(T2 lhs, const TVector<T1, D> &rhs) {
  return TVector<T1, D>(lhs) / rhs;
}

// Math functions
template <typename Scalar, int Dimension>
Scalar dot(const TVector<Scalar, Dimension> &lhs, const TVector<Scalar, Dimension> &rhs) {
  return lhs.dot(rhs);
}

template <typename Scalar, int Dimension>
TVector<Scalar, Dimension> cross(const TVector<Scalar, Dimension> &lhs, const TVector<Scalar, Dimension> &rhs) {
  return lhs.cross(rhs);
}

template <typename T, int D>
TVector<T, D> cwise_max(const TVector<T, D> &vec, T val) {
  return vec.cwiseMax(TVector<T, D>(val));
}

template <typename T, int D>
TVector<T, D> cwise_max(const TVector<T, D> &lhs, const TVector<T, D> &rhs) {
  return lhs.cwiseMax(rhs);
}

template <typename T, int D>
TVector<T, D> cwise_min(const TVector<T, D> &vec, T val) {
  return vec.cwiseMax(TVector<T, D>(val));
}

template <typename T, int D>
TVector<T, D> cwise_min(const TVector<T, D> &lhs, const TVector<T, D> &rhs) {
  return lhs.cwiseMax(rhs);
}

template <typename T, int D>
TVector<int, D> ceil2int(const TVector<T, D> &vec) {
  return TVector<int, D>(TVector<T, D>(vec.array().ceil().matrix()));
}

template <typename T, int D>
TVector<int, D> floor2int(const TVector<T, D> &vec) {
  return TVector<int, D>(TVector<T, D>(vec.array().floor().matrix()));
}

template <typename Scalar, int Dimension>
auto hprod(const TVector<Scalar, Dimension> &v) {
  return v.prod();
}

template <typename Scalar, int Dimension>
auto hsum(const TVector<Scalar, Dimension> &v) {
  return v.sum();
}

template <typename Scalar, int Dimension>
auto hmax(const TVector<Scalar, Dimension> &v) {
  return v.maxCoeff();
}

template <typename Scalar, int Dimension>
auto hmin(const TVector<Scalar, Dimension> &v) {
  return v.minCoeff();
}

template <typename Scalar, int Dimension>
auto norm(const TVector<Scalar, Dimension> &v) {
  return v.norm();
}

template <typename Scalar, int Dimension>
auto squared_norm(const TVector<Scalar, Dimension> &v) {
  return v.squaredNorm();
}

template <typename Scalar, typename Scalar2, int Dimension>
auto clamp(const TVector<Scalar, Dimension> &v, Scalar2 vmin, Scalar2 vmax) {
  return TVector<Scalar, Dimension>(v.cwiseMin(TVector<Scalar, Dimension>(vmax)).cwiseMax(TVector<Scalar, Dimension>(vmin)));
}

// Compute orthonormal basis
// Based on paper "Building an Orthonormal Basis, Revisited"
template <typename Float>
std::pair<TVector<Float, 3>, TVector<Float, 3>> coordinate_system(const TVector<Float, 3> &n) {
  Float sign = std::copysign(1.f, n.z());
  const Float a = -1.f / (sign + n.z());
  const Float b = n.x() * n.y() * a;
  return {
      {1.f + sign * n.x() * n.x() * a, sign * b, -sign * n.x()},
      {b, sign + n.y() * n.y() * a, -n.y()}};
}

}  // namespace misaki::math