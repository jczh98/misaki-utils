#pragma once

#include "vec2.hpp"

namespace misaki::math {

template <typename Value>
struct TBoundingBox2 {
  using PointType = TVector2<Value>;

  MSK_CPU_GPU TBoundingBox2() { reset(); }
  MSK_CPU_GPU TBoundingBox2(const PointType &p) {
    this->pmin = p;
    this->pmax = p;
  }
  MSK_CPU_GPU TBoundingBox2(const PointType &pmin, const PointType &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }

  MSK_CPU_GPU void reset() {
    pmin = (std::numeric_limits<Value>::infinity());
    pmax = (-std::numeric_limits<Value>::infinity());
  }

  template <typename T>
  MSK_CPU_GPU void clip(const TBoundingBox2<T> &bbox) {
    pmin = cwise_max(pmin, bbox.pmin);
    pmax = cwise_min(pmax, bbox.pmax);
  }

  template <typename T>
  MSK_CPU_GPU void expand(const TVector2<T> &p) {
    pmin = cwise_min(pmin, p);
    pmax = cwise_max(pmax, p);
  }

  template <typename T>
  MSK_CPU_GPU void expand(const TBoundingBox2<T> &bbox) {
    pmin = cwise_min(pmin, bbox.pmin);
    pmax = cwise_max(pmax, bbox.pmax);
  }

  MSK_CPU_GPU PointType center() const {
    return (pmin + pmax) * Value(.5f);
  }

  PointType pmin, pmax;
};

using BoundingBox2f = TBoundingBox2<float>;
using BoundingBox2d = TBoundingBox2<double>;
using BoundingBox2i = TBoundingBox2<int>;

}  // namespace misaki::math