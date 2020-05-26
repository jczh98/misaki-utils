#pragma once

#include <sstream>

#include "../misc/string.h"
#include "vector.h"

namespace misaki::math {

template <typename Float>
struct Frame {
  using Vector3 = TVector<Float, 3>;

  Vector3 s, t, n;

  Frame(const Vector3 &v) : n(v) {
    std::tie(s, t) = coordinate_system<Float>(v);
  }

  Vector3 to_local(const Vector3 &v) const {
    return {dot(v, s), dot(v, t), dot(v, n)};
  }

  Vector3 to_world(const Vector3 &v) const {
    return s * v.x() + t * v.y() + n * v.z();
  }

  std::string to_string() const {
    std::ostringstream os;
    os << "Frame[" << std::endl
       << "  s = " << string::indent(s.to_string(), 6) << "," << std::endl
       << "  t = " << string::indent(t.to_string(), 6) << "," << std::endl
       << "  n = " << string::indent(n.to_string(), 6) << std::endl
       << "]";
    return os.str();
  }

  static Float cos_theta(const Vector3 &v) { return v.z(); }
};

template <typename Float>
std::ostream &operator<<(std::ostream &os, const Frame<Float> &f) {
  os << "Frame[" << std::endl
     << "  s = " << string::indent(f.s.to_string(), 6) << "," << std::endl
     << "  t = " << string::indent(f.t.to_string(), 6) << "," << std::endl
     << "  n = " << string::indent(f.n.to_string(), 6) << std::endl
     << "]";
  return os;
}

}  // namespace misaki::math