#pragma once

#include "tuple3.hpp"

namespace misaki {
namespace math {

template <typename T>
class BoundingBox3 {
 public:
  MSK_CPU_GPU BoundingBox3() {
    pmin = (std::numeric_limits<Value>::infinity());
    pmax = (-std::numeric_limits<Value>::infinity());
  }
  MSK_CPU_GPU explicit BoundingBox3(const Point3<T> &p) : pmin(p), pmax(p) {}
  MSK_CPU_GPU BoundingBox3(const Point3<T> &pmin, const Point3<T> &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }
  Point3<T> pmin, pmax;
};

}  // namespace math
}  // namespace misaki