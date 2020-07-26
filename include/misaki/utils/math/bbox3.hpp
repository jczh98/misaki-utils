#pragma once

#include "vec3.hpp"

namespace misaki::math {

template <typename Value>
struct TBoundingBox3 {
  using PointType = TVector3<Value>;

  TBoundingBox3() { reset(); }
  TBoundingBox3(const PointType &p) {
    this->pmin = p;
    this->pmax = p;
  }
  TBoundingBox3(const PointType &pmin, const PointType &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }

  void reset() {
    pmin = (std::numeric_limits<Value>::infinity());
    pmax = (-std::numeric_limits<Value>::infinity());
  }

  template <typename T>
  void clip(const TBoundingBox3<T> &bbox) {
    pmin = cwise_max(pmin, bbox.pmin);
    pmax = cwise_min(pmax, bbox.pmax);
  }

  template <typename T>
  void expand(const TVector3<T> &p) {
    pmin = cwise_min(pmin, p);
    pmax = cwise_max(pmax, p);
  }

  template <typename T>
  void expand(const TBoundingBox3<T> &bbox) {
    pmin = cwise_min(pmin, bbox.pmin);
    pmax = cwise_max(pmax, bbox.pmax);
  }

  PointType center() const {
    return (pmin + pmax) * Value(.5f);
  }

  PointType pmin, pmax;
};

using BoundingBox3f = TBoundingBox3<float>;
using BoundingBox3d = TBoundingBox3<double>;

}  // namespace misaki::math