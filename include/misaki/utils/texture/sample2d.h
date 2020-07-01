#include "../math.h"

namespace misaki::texture {

template <typename Float, typename TensorType>
TensorType nearest_sample2d(const math::TVector<Float, 2> &uv, const math::Tensor<TensorType, 2> &bitmap) {
  const int width = bitmap.shape()[1], height = bitmap.shape()[0];
  const int x = math::clamp(int(uv.x() * width), 0, width - 1);
  const int y = math::clamp(int(uv.y() * height), 0, height - 1);
  return bitmap.at({y, x});
}

}  // namespace misaki::texture