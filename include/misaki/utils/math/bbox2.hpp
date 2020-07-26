#pragma once

#include "vec2.hpp"

namespace misaki::math {

template <typename Value>
struct TBoundingBox2 {
  using PointType = TVector2<Value>;

  TBoundingBox2() { reset(); }
  TBoundingBox2(const PointType &p) {
    this->pmin = p;
    this->pmax = p;
  }
  TBoundingBox2(const PointType &pmin, const PointType &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }

  void reset() {
    pmin = (std::numeric_limits<Value>::infinity());
    pmax = (-std::numeric_limits<Value>::infinity());
  }

  template <typename T>
  void clip(const TBoundingBox<T, Size> &bbox) {
    pmin = cwise_max(pmin, bbox.pmin);
    pmax = cwise_min(pmax, bbox.pmax);
  }

  template <typename T>
  void expand(const TVector<T, Size> &p) {
    pmin = cwise_min(pmin, p);
    pmax = cwise_max(pmax, p);
  }

  template <typename T>
  void expand(const TBoundingBox<T, Size> &bbox) {
    pmin = cwise_min(pmin, bbox.pmin);
    pmax = cwise_max(pmax, bbox.pmax);
  }

  PointType center() const {
    return (pmin + pmax) * Value(.5f);
  }

  PointType pmin, pmax;
};

using BoundingBox2f = TBoundingBox2<float>;
using BoundingBox2d = TBoundingBox2<double>;

}  // namespace misaki::math