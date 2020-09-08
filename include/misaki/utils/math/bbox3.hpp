#pragma once

#include "vec3.hpp"

namespace misaki::math {

template <typename Value>
struct TBoundingBox3 {
  using PointType = TVector3<Value>;

  MSK_XPU TBoundingBox3() { reset(); }
  MSK_XPU TBoundingBox3(const PointType &p) {
    this->pmin = p;
    this->pmax = p;
  }
  MSK_XPU TBoundingBox3(const PointType &pmin, const PointType &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }

  MSK_XPU void reset() {
    pmin = (std::numeric_limits<Value>::infinity());
    pmax = (-std::numeric_limits<Value>::infinity());
  }

  template <typename T>
  MSK_XPU void clip(const TBoundingBox3<T> &bbox) {
    pmin = cwise_max(pmin, bbox.pmin);
    pmax = cwise_min(pmax, bbox.pmax);
  }

  template <typename T>
  MSK_XPU void expand(const TVector3<T> &p) {
    pmin = cwise_min(pmin, p);
    pmax = cwise_max(pmax, p);
  }

  template <typename T>
  MSK_XPU void expand(const TBoundingBox3<T> &bbox) {
    pmin = cwise_min(pmin, bbox.pmin);
    pmax = cwise_max(pmax, bbox.pmax);
  }

  MSK_XPU PointType center() const {
    return (pmin + pmax) * Value(.5f);
  }

  PointType pmin, pmax;

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

// Stream
template <typename Value>
std::ostream &operator<<(
    std::ostream &oss, const TBoundingBox3<Value> &bbox) {
  oss << "[";
  oss << bbox.pmin << ",\n ";
  oss << bbox.pmax;
  oss << "]";
  return oss;
}

using BoundingBox3f = TBoundingBox3<float>;
using BoundingBox3d = TBoundingBox3<double>;

}  // namespace misaki::math