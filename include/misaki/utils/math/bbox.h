#pragma once

#include "vector.h"

namespace misaki::math {

template <typename Value, int Size>
struct TBoundingBox {
  using PointType = TVector<Value, Size>;
  TBoundingBox() { reset(); }
  TBoundingBox(const PointType &p) {
    this->pmin = p;
    this->pmax = p;
  }
  TBoundingBox(const PointType &pmin, const PointType &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }

  void reset() {
    pmin.setConstant(std::numeric_limits<Value_>::infinity());
    pmax.setConstant(-std::numeric_limits<Value_>::infinity());
  }

  template <typename T>
  void clip(const TBoundingBox<T, Size> &bbox) {
    pmin = pmin.cwiseMax(bbox.pmin);
    pmax = pmax.cwiseMin(bbox.pmax);
  }

  template <typename T>
  void expand(const TVector<T, Size> &p) {
    pmin = pmin.cwiseMin(p);
    pmax = pmax.cwiseMax(p);
  }

  template <typename T>
  void expand(const TBoundingBox<T, Size> &bbox) {
    pmin = pmin.cwiseMin(bbox.pmin);
    pmax = pmax.cwiseMax(bbox.pmax);
  }

  PointType center() const {
    return (pmin + pmax) * Value(.5f);
  }

  PointType pmin, pmax;
};

}  // namespace misaki::math