#pragma once

#include "tuple2.hpp"

namespace misaki {
namespace math {

template <typename T>
class BoundingBox2 {
 public:
  MSK_CPU_GPU BoundingBox2() {
    pmin = (std::numeric_limits<Value>::infinity());
    pmax = (-std::numeric_limits<Value>::infinity());
  }
  MSK_CPU_GPU explicit BoundingBox2(const Point2<T> &p) : pmin(p), pmax(p) {}
  MSK_CPU_GPU BoundingBox3(const Point2<T> &pmin, const Point2<T> &pmax) {
    this->pmin = pmin;
    this->pmax = pmax;
  }
  Point2<T> pmin, pmax;
};

}  // namespace math
}  // namespace misaki