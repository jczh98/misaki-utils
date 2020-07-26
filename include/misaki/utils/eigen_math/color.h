#pragma once

#include "transform.h"
#include "vector.h"

namespace misaki::math {

template <typename Value, int Size>
struct TColor;

template <typename Value_>
struct TColor<Value_, 3> : Eigen::Array<Value_, 3, 1> {
  using Base = Eigen::Array<Value_, 3, 1>;

  TColor(Value_ value = 0) : Base(value, value, value) {}
  template <typename T>
  TColor(T r, T g, T b) : Base(r, g, b) {}
  template <typename Derived>
  TColor(const Eigen::ArrayBase<Derived> &p) : Base(p) {}
  template <typename Derived>
  TColor &operator=(const Eigen::ArrayBase<Derived> &p) {
    this->Base::operator=(p);
    return *this;
  }

  decltype(auto) r() const { return x(); }
  decltype(auto) r() { return x(); }

  decltype(auto) g() const { return y(); }
  decltype(auto) g() { return y(); }

  decltype(auto) b() const { return z(); }
  decltype(auto) b() { return z(); }

  Value_ luminance() const {
    return coeff(0) * 0.212671f + coeff(1) * 0.715160f + coeff(2) * 0.072169f;
  }

  TColor<Value_, 3> to_srgb() const {
    TColor<Value_, 3> result;
    for (int i = 0; i < 3; ++i) {
      float value = coeff(i);
      if (value <= 0.0031308f)
        result[i] = 12.92f * value;
      else
        result[i] = (1.0f + 0.055f) * std::pow(value, 1.0f / 2.4f) - 0.055f;
    }
    return result;
  }

  TColor<Value_, 3> to_linear() const {
    TColor<Value_, 3> result;
    for (int i = 0; i < 3; ++i) {
      float value = coeff(i);
      if (value <= 0.04045f)
        result[i] = value * (1.0f / 12.92f);
      else
        result[i] = std::pow((value + 0.055f) * (1.0f / 1.055f), 2.4f);
    }
    return result;
  }

  bool is_valid() const {
    for (int i = 0; i < 3; i++) {
      Value_ value = coeff(i);
      if (value < 0 || !std::isfinite(value)) return false;
    }
    return true;
  }

  bool operator==(const TColor &p) const {
    return p.r() == r() && p.g() == g() && p.b() == b();
  }
  bool operator!=(const TColor &p) const {
    return !operator==(p);
  }

  std::string to_string() const {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < 3; i++) {
      oss << coeff(i);
      oss << (i == 2 ? "" : ", ");
    }
    oss << "]";
    return oss.str();
  }
};

template <typename Value_>
struct TColor<Value_, 4> : Eigen::Array<Value_, 4, 1> {
  using Base = Eigen::Array<Value_, 4, 1>;

  template <typename T_>
  TColor(std::initializer_list<T_> l) : Base(l) {}
  TColor() : Base(0, 0, 0, 0) {}
  template <typename T>
  TColor(T r, T g, T b, T a) : Base(r, g, b, a) {}
  TColor(const TColor<Value_, 3> &c) : Base(c.r(), c.g(), c.b(), 1.0f) {}

  decltype(auto) r() const { return x(); }
  decltype(auto) r() { return x(); }

  decltype(auto) g() const { return y(); }
  decltype(auto) g() { return y(); }

  decltype(auto) b() const { return z(); }
  decltype(auto) b() { return z(); }

  decltype(auto) a() const { return w(); }
  decltype(auto) a() { return w(); }

  bool is_valid() const {
    for (int i = 0; i < 4; i++) {
      Value value = coeff(i);
      if (value < 0 || !std::isfinite(value)) return false;
    }
    return true;
  }

  TColor<Value_, 3> divide_by_filter_weight() const {
    if (w() != 0)
      return head<3>() / w();
    else
      return TColor<Value_, 3>(0.0f);
  }

  std::string to_string() const {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < 4; i++) {
      oss << coeff(i);
      oss << (i == 3 ? "" : ", ");
    }
    oss << "]";
    return oss.str();
  }
};

template <typename Value, int Size, std::enable_if_t<(Size == 4 || Size == 3), int> = 0>
std::ostream &operator<<(
    std::ostream &out, const TColor<Value, Size> &c) {
  out << c.to_string();
  return out;
}

template <typename Float>
TColor<Float, 3> linear_to_srgb(const TColor<Float, 3> &xyz) {
  TVector<Float, 3> result = TVector<Float, 3>(xyz.r(), xyz.g(), xyz.b());
  TMatrix<Float, 3> M;
  M << 3.240479f, -1.537150f, -0.498535f,
      -0.969256f, 1.875991f, 0.041556f,
      0.055648f, -0.204043f, 1.057311f;
  return M * result;
}

// Convert ITU-R Rec. BT.709 linear RGB to XYZ tristimulus values
template <typename Float>
TVector<Float, 3> srgb_to_xyz(const TColor<Float, 3> &rgb) {
  TVector<Float, 3> result = TVector<Float, 3>(rgb.r(), rgb.g(), rgb.b());
  TMatrix<Float, 3> M;
  M << 0.412453f, 0.357580f, 0.180423f,
      0.212671f, 0.715160f, 0.072169f,
      0.019334f, 0.119193f, 0.950227f;
  return M * result;
}

template <typename Float>
Float luminance(const TColor<Float, 3> &c) {
  return c[0] * 0.212671f + c[1] * 0.715160f + c[2] * 0.072169f;
}

template <typename Float, int N>
auto lerp(const TColor<Float, N> &a, const TColor<Float, N> &b, const TColor<Float, N> &t) {
  return (1 - t) * a + t * b;
}

}  // namespace misaki::math