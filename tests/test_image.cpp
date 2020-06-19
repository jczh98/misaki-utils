#include <misaki/utils/image.h>

#include <iostream>
using namespace misaki;

int main() {
  auto bitmap = math::Tensor<math::TColor<float, 3>, 2>::from_linear_indexed(
      {320, 640},
      [&](int i) {
        int y = i / 640, x = i % 640;
        auto col = math::TColor<float, 3>((float)x / 640, (float)y / 320, 0.2f);
        return math::TColor<float, 3>(0.0, 0.0, 1.0);
      });
  image::write_rgb_image("test.png", bitmap);
}