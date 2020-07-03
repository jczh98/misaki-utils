#pragma once

#include "vector.h"

namespace misaki::math {

template <typename Value_, int Size_>
struct TBoundingSphere {
  using Value = Value_;
  using PointType = TVector<Value_, Size_>;
  PointType center;
  Value radius;
  TBoundingSphere() : center(0.f), radius(0.f) {}
  TBoundingSphere(const PointType &center, const Value &radius) : center(center), radius(radius) {}

  bool operator==(const TBoundingSphere &bsphere) const {
    return center == bsphere.center && radius == bsphere.radius;
  }

  bool operator!=(const TBoundingSphere &bsphere) const {
    return center != bsphere.center || radius != bsphere.radius;
  }

  bool empty() const { return radius <= 0.f; }
  void expand(const PointType &p) {
    radius = std::max<Value>(radius, norm(p - center));
  }
  bool contains(const PointType &p, bool strict = false) const {
    if (strict)
      return squared_norm(p - center) < radius * radius;
    else
      return squared_norm(p - center) <= radius * radius
  }
};

}  // namespace misaki::math