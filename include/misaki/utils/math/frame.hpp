#pragma once

#include "../misc/string.h"
#include "vec2.hpp"
#include "vec3.hpp"

namespace misaki::math {

template <typename Float>
struct TFrame {
  using Vector3 = TVector3<Float>;
  using Vector2 = TVector2<Float>;

  Vector3 s, t, n;

  MSK_XPU TFrame(const Vector3 &v) : n(v) {
    std::tie(s, t) = coordinate_system<Float>(v);
  }

  MSK_XPU Vector3 to_local(const Vector3 &v) const {
    return {dot(v, s), dot(v, t), dot(v, n)};
  }

  MSK_XPU Vector3 to_world(const Vector3 &v) const {
    return s * v.x + t * v.y + n * v.z;
  }

  MSK_XPU static Float cos_theta(const Vector3 &v) { return v.z; }
  MSK_XPU static Float cos_theta_2(const Vector3 &v) { return math::sqr(v.z); }
  MSK_XPU static Float sin_theta(const Vector3 &v) {
    return math::safe_sqrt(sin_theta_2(v));
  }
  MSK_XPU static Float sin_theta_2(const Vector3 &v) {
    return math::sqr(v.x) + math::sqr(v.y);
  }
  MSK_XPU static Float tan_theta(const Vector3 &v) {
    return math::safe_sqrt(1.f - math::sqr(v.z)) / v.z;
  }
  MSK_XPU static Float tan_theta_2(const Vector3 &v) {
    return std::max(0.f, 1.f - math::sqr(v.z)) / math::sqr(v.z);
  }
  MSK_XPU static Float sin_phi(const Vector3 &v) {
    Float sin_theta_2 = TFrame::sin_theta_2(v),
          inv_sin_theta = math::safe_rsqrt(TFrame::sin_theta_2(v));
    return std::abs(sin_theta_2) <= 4.f * Epsilon<Float>
               ? 0.f
               : std::clamp(v.y * inv_sin_theta, -1.f, 1.f);
  }
  MSK_XPU static Float cos_phi(const Vector3 &v) {
    Float sin_theta_2 = TFrame::sin_theta_2(v),
          inv_sin_theta = math::safe_rsqrt(TFrame::sin_theta_2(v));
    return std::abs(sin_theta_2) <= 4.f * Epsilon<Float>
               ? 1.f
               : std::clamp(v.x * inv_sin_theta, -1.f, 1.f);
  }
  static std::pair<Float, Float> sincos_phi(const Vector3 &v) {
    Float sin_theta_2 = TFrame::sin_theta_2(v),
          inv_sin_theta = math::safe_rsqrt(TFrame::sin_theta_2(v));

    Vector2 result = {v.x * inv_sin_theta, v.y * inv_sin_theta};

    result = std::abs(sin_theta_2) <= 4.f * Epsilon<Float>
                 ? Vector2(1.f, 0.f)
                 : std::clamp(result, -1.f, 1.f);

    return {result.y, result.x};
  }
};

template <typename Float>
std::ostream &operator<<(std::ostream &os, const TFrame<Float> &f) {
  os << "Frame[" << std::endl
     << "  s = " << f.s << "," << std::endl
     << "  t = " << f.t << "," << std::endl
     << "  n = " << f.n << std::endl
     << "]";
  return os;
}

// Type alias
using Frame3f = TFrame<float>;
using Frame3d = TFrame<double>;

}  // namespace misaki::math