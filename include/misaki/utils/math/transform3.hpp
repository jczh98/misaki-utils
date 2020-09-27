#pragma once

#include "../misc/string.h"
#include "matrix3.hpp"

namespace misaki::math {

template <typename Value>
class TTransform3 {
  using Matrix3 = TMatrix3<Value>;
  using Vector2 = TVector2<Value>;
  using Self = TTransform3<Value>;
  Matrix3 m_matrix = Matrix3::identity(),
          m_inverse_matrix = Matrix3::identity();

 public:
  MSK_XPU TTransform3() = default;
  MSK_XPU explicit TTransform3(const Matrix3 &m)
      : m_matrix(m), m_inverse_matrix(m.inverse()) {}
  MSK_XPU TTransform3(const Matrix3 &m, const Matrix3 &inv_m)
      : m_matrix(m), m_inverse_matrix(inv_m) {}

  // Component access
  MSK_XPU const Matrix3 &matrix() const { return m_matrix; }
  MSK_XPU const Matrix3 &inverse_matrix() const { return m_inverse_matrix; }

  MSK_XPU Self inverse() const { return Self(m_inverse_matrix, m_matrix); }

  MSK_XPU Vector2 apply_point(const Vector2 &point) const noexcept {
    using Vector3 = TVector3<Value>;
    const auto p = m_matrix * Vector3(point.x, point.y, 1);
    return Vector2(p.x, p.y) / p.z;
  }

  MSK_XPU Vector2 apply_vector(const Vector2 &vec) const noexcept {
    using Vector3 = TVector3<Value>;
    const auto p = m_matrix * Vector3(vec.x, vec.y, 0);
    return Vector2(p.x, p.y);
  }

  MSK_XPU Vector2 apply_normal(const Vector2 &normal) const noexcept {
    using Vector3 = TVector3<Value>;
    const auto p = (m_inverse_matrix * Vector3(normal.x, normal.y, 0));
    return Vector2(p.x, p.y).normalize();
  }

  MSK_XPU static Self translate(const Vector2 &delta) noexcept {
    Matrix3 m(1, 0, delta.x, 0, 1, delta.y, 0, 0, 1);
    Matrix3 minv(1, 0, -delta.x, 0, 1, -delta.y, 0, 0, 1);
    return Self(m, minv);
  }

  MSK_XPU static Self scale(const Vector2 &v) noexcept {
    Matrix3 m(v.x, 0, 0, 0, v.y, 0, 0, 0, 1);
    Matrix3 minv(1.f / v.x, 0, 0, 0, 1.f / v.y, 0, 0, 0, 1);
    return Self(m, minv);
  }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

template <typename Value>
std::ostream &operator<<(std::ostream &oss, const TTransform3<Value> &t) {
  oss << "Transform[" << std::endl;
  oss << "  matrix = " << string::indent(t.matrix().to_string(), 11) << ","
      << std::endl;
  oss << "  inverse_matrix = "
      << string::indent(t.inverse_matrix().to_string(), 19) << "," << std::endl;
  oss << "]";
  return oss;
}

template <typename Value>
MSK_XPU TTransform3<Value> operator*(const TTransform3<Value> &lhs,
                                     const TTransform3<Value> &rhs) noexcept {
  return TTransform3<Value>(lhs.matrix() * rhs.matrix(),
                            lhs.inverse_matrix() * rhs.inverse_matrix();
}

// Type alias
using Transform3f = TTransform3<float>;
using Transform3d = TTransform3<double>;

}  // namespace misaki::math