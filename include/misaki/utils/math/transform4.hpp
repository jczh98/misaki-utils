#pragma once

#include "../misc/string.h"
#include "matrix4.hpp"

namespace misaki::math {

template <typename Value>
class TTransform4 {
  using Matrix4 = TMatrix4<Value>;
  using Vector3 = TVector3<Value>;
  using Self = TTransform4<Value>;
  Matrix4 m_matrix = Matrix4::identity(), m_inverse_matrix = Matrix4::identity();

 public:
  TTransform4() = default;
  explicit TTransform4(const Matrix4 &m) : m_matrix(m), m_inverse_matrix(m.inverse()) {}
  TTransform4(const Matrix4 &m, const Matrix4 &inv_m) : m_matrix(m), m_inverse_matrix(inv_m) {}

  // Component access
  const Matrix4 &matrix() const { return m_matrix; }
  const Matrix4 &inverse_matrix() const { return m_inverse_matrix; }

  Self inverse() const { return Self(m_inverse_matrix, m_matrix); }

  Vector3 apply_point(const Vector3 &point) const noexcept {
    using Vector4 = TVector4<Value>;
    const auto p = m_matrix * Vector4(point.x, point.y, point.z, 1);
    return Vector3(p.x, p.y, p.z) / p.w;
  }

  Vector3 apply_vector(const Vector3 &vec) const noexcept {
    using Vector4 = TVector4<Value>;
    const auto p = m_matrix * Vector4(vec.x, vec.y, vec.z, 0);
    return Vector3(p.x, p.y, p.z);
  }

  Vector3 apply_normal(const Vector3 &normal) const noexcept {
    using Vector4 = TVector4<Value>;
    const auto p = (m_inverse_matrix * Vector4(normal.x, normal.y, normal.z, 0));
    return Vector3(p.x, p.y, p.z).normalize();
  }

  static Self translate(const Vector3 &delta) noexcept {
    Matrix4 m(1, 0, 0, delta.x, 0, 1, 0, delta.y, 0, 0, 1, delta.z, 0, 0, 0,
              1);
    Matrix4 minv(1, 0, 0, -delta.x, 0, 1, 0, -delta.y, 0, 0, 1, -delta.z, 0,
                 0, 0, 1);
    return Self(m, minv);
  }

  static Self scale(const Vector3 &v) noexcept {
    Matrix4 m(v.x, 0, 0, 0, 0, v.y, 0, 0, 0, 0, v.z, 0, 0, 0, 0, 1);
    Matrix4 minv(1 / v.x, 0, 0, 0, 0, 1 / v.y, 0, 0, 0, 0, 1 / v.z, 0, 0, 0, 0, 1);
    return Self(m, minv);
  }

  static Self rotate(const Vector3 &axis, Value angle) noexcept {
    const Vector3 a = axis.normalize();
    Value sin_theta = std::sin(deg_to_rag(angle));
    Value cos_theta = std::cos(deg_to_rag(angle));
    Matrix4 m;

    m[0][0] = a.x * a.x + (1 - a.x * a.x) * cos_theta;
    m[0][1] = a.x * a.y * (1 - cos_theta) - a.z * sin_theta;
    m[0][2] = a.x * a.z * (1 - cos_theta) + a.y * sin_theta;
    m[0][3] = 0;

    m[1][0] = a.x * a.y * (1 - cos_theta) + a.z * sin_theta;
    m[1][1] = a.y * a.y + (1 - a.y * a.y) * cos_theta;
    m[1][2] = a.y * a.z * (1 - cos_theta) - a.x * sin_theta;
    m[1][3] = 0;

    m[2][0] = a.x * a.z * (1 - cos_theta) - a.y * sin_theta;
    m[2][1] = a.y * a.z * (1 - cos_theta) + a.x * sin_theta;
    m[2][2] = a.z * a.z + (1 - a.z * a.z) * cos_theta;
    m[2][3] = 0;
    return Self(m, m.transpose());
  }

  static Self look_at(const Vector3 &origin,
                     const Vector3 &target,
                     const Vector3 &up) noexcept {
    auto dir = (target - origin).normalize();
    auto left = cross(dir, up.normalize()).normalize();
    auto new_up = cross(dir, left).normalize();
    Matrix4 result = Matrix4::from_cols(
        {left.x, left.y, left.z, 0},
        {new_up.x, new_up.y, new_up.z, 0},
        {dir.x, dir.y, dir.z, 0},
        {origin.x, origin.y, origin.z, 1});
    return Self(result);
  }

  static Self perspective(Value fov, Value near_, Value far_) noexcept {
    Value recip = 1.0f / (far_ - near_),
          cot = 1.0f / std::tan(deg_to_rag(fov / 2.0f));
    Matrix4 persp(cot, 0, 0, 0,
                  0, cot, 0, 0,
                  0, 0, far_ * recip, -near_ * far_ * recip,
                  0, 0, 1, 0);
    return Self(persp);
  }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

template <typename Value>
std::ostream &operator<<(std::ostream &oss, const TTransform4<Value> &t) {
  oss << "Transform[" << std::endl;
  oss << "  matrix = " << string::indent(t.matrix().to_string(), 11) << "," << std::endl;
  oss << "  inverse_matrix = " << string::indent(t.inverse_matrix().to_string(), 19) << "," << std::endl;
  oss << "]";
  return oss;
}

// Type alias
using Transform4f = TTransform4<float>;
using Transform4d = TTransform4<double>;

}  // namespace misaki::math