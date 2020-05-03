#pragma once

#include <type_traits>
#include <Eigen/Dense>

namespace fuka {

// ---------------------------Vector-------------------------------

template<typename Scalar, int Dimension>
struct TVector : public Eigen::Matrix<Scalar, Dimension, 1> {
 public:
  using Base = Eigen::Matrix<Scalar, Dimension, 1>;
  using Self = TVector<Scalar, Dimension>;

  template<typename Value, std::enable_if_t<std::is_arithmetic<Value>::value, int> = 0>
  TVector(Value value = (Value) 0) { Base::setConstant(Scalar(value)); }

  template<typename ...Args, std::enable_if_t<sizeof...(Args) == Dimension, int> = 0>
  TVector(Args &&...args) : Base(std::forward<Args>(args)...) {};

  template<typename T>
  TVector(const TVector<T, Dimension> &vec) : Base(vec.cast<Scalar>()) {};

  template<typename Derived>
  TVector(const Eigen::MatrixBase<Derived> &p)
      : Base(p) {}

  template<typename Derived>
  TVector &operator=(const Eigen::MatrixBase<Derived> &p) {
    this->Base::operator=(p);
    return *this;
  }

  Self operator+(const TVector<Scalar, Dimension> &rhs) {
    Base rhs_ = rhs;
    return TVector<Scalar, Dimension>(Base(*this) + rhs_);
  }
  Self operator-(const TVector<Scalar, Dimension> &rhs) {
    Base rhs_ = rhs;
    return TVector<Scalar, Dimension>(Base(*this) - rhs_);
  }
  Self operator*(const TVector<Scalar, Dimension> &rhs) {
    return TVector<Scalar, Dimension>(this->cwiseProduct(rhs));
  }
  Self operator/(const TVector<Scalar, Dimension> &rhs) {
    return TVector<Scalar, Dimension>((this->array() / rhs.array()).matrix());
  }

  template<typename T> Self operator+(T rhs) {
    return this->operator+(Self(rhs));
  }
  template<typename T> Self operator-(T rhs) {
    return this->operator-(Self(rhs));
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

template<typename Scalar, int Dimension>
std::ostream &operator<<(
    std::ostream &out, const TVector<Scalar, Dimension> &vec) {
  out << vec.to_string();
  return out;
}

template<typename T1, typename T2, int Dimension>
TVector<T1, Dimension> operator+(T2 lhs, const TVector<T1, Dimension> &rhs) {
  return TVector<T1, Dimension>(lhs) + rhs;
}

template<typename T1, typename T2, int Dimension>
TVector<T1, Dimension> operator-(T2 lhs, const TVector<T1, Dimension> &rhs) {
  return TVector<T1, Dimension>(lhs) - rhs;
}

template<typename T1, typename T2, int Dimension>
TVector<T1, Dimension> operator*(T2 lhs, const TVector<T1, Dimension> &rhs) {
  return TVector<T1, Dimension>(lhs) * rhs;
}

template<typename T1, typename T2, int Dimension>
TVector<T1, Dimension> operator/(T2 lhs, const TVector<T1, Dimension> &rhs) {
  return TVector<T1, Dimension>(lhs) / rhs;
}

}