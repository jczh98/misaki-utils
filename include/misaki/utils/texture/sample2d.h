#include "../math.h"

namespace misaki::texture {

template <typename Float, typename TensorType>
TensorType nearest_sample2d(const math::TVector<Float, 2> &uv, const math::Tensor<TensorType, 2> &bitmap) {
  const int width = bitmap.shape()[1], height = bitmap.shape()[0];
  const int x = math::clamp(int(uv.x() * width), 0, width - 1);
  const int y = math::clamp(int(uv.y() * height), 0, height - 1);
  return bitmap.at({y, x});
}

template <typename Float, typename TensorType>
TensorType linear_sample2d(const math::TVector<Float, 2> &uv, const math::Tensor<TensorType, 2> &bitmap) {
  const int width = bitmap.shape()[1], height = bitmap.shape()[0];
  const auto fu = uv.x() * width;
  const auto fv = uv.y() * height;
  const int pu = math::clamp(static_cast<int>(fu), 0, width - 1);
  const int pv = math::clamp(static_cast<int>(fv), 0, height - 1);
  const int dpu = (fu > pu + 0.5) ? 1 : -1;
  const int dpv = (fv > pv + 0.5) ? 1 : -1;
  const int apu = math::clamp(pu + dpu, 0, width - 1);
  const int apv = math::clamp(pv + dpv, 0, height - 1);
  const Float du = std::min(std::abs(fu - pu - 0.5f), 1.f);
  const Float dv = std::min(std::abs(fv - pv - 0.5f), 1.f);
  const auto pupv = bitmap.at({pv, pu});
  const auto apupv = bitmap.at({pv, apu});
  const auto puapv = bitmap.at({apv, pu});
  const auto apuapv = bitmap.at({apv, apu});
  return (pupv * (1.f - du) + apupv * du) * (1.f - dv) +
         (puapv * (1.f - du) + apuapv * du) * dv;
}

}  // namespace misaki::texture