#include <misaki/utils/image/imageio.h>

namespace {
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
}  // namespace

namespace misaki::image {

using Color3 = math::TColor<float, 3>;
using Color4 = math::TColor<float, 4>;

math::Tensor<Color3, 2> read_rgb_image(const std::string &filename) {
  int w, h, channel;
  float *rgbs = stbi_loadf(filename.c_str(), &w, &h, &channel, 3);
  return math::Tensor<Color3, 2>::from_linear_indexed({h, w}, [&](int i) {
    auto idx = 3 * i;
    return Color3(rgbs[idx], rgbs[idx + 1], rgbs[idx + 2]);
  });
}

math::Tensor<Color4, 2> read_rgba_image(const std::string &filename) {
  int w, h, channel;
  float *rgbas = stbi_loadf(filename.c_str(), &w, &h, &channel, 4);
  return math::Tensor<Color4, 2>::from_linear_indexed({h, w}, [&](int i) {
    auto idx = 4 * i;
    return Color4(rgbas[idx], rgbas[idx + 1], rgbas[idx + 2], rgbas[idx + 3]);
  });
}

void write_rgb_image(const std::string &filename, const math::Tensor<math::TColor<float, 3>, 2> &data) {
  int width = data.shape()[1], height = data.shape()[0];
  uint8_t *rgbs = new uint8_t[width * height * 3];
  uint8_t *dst = rgbs;
  for (int y = height - 1; y >= 0; --y)
    for (int x = 0; x < width; ++x) {
      dst[0] = (uint8_t)math::clamp(data.at({y, x})[0] * 255.0, 0., 255.);
      dst[1] = (uint8_t)math::clamp(data.at({y, x})[1] * 255.0, 0., 255.);
      dst[2] = (uint8_t)math::clamp(data.at({y, x})[2] * 255.0, 0., 255.);
      dst += 3;
    }

  if (!stbi_write_png(filename.c_str(), width, height, 3, rgbs, 3 * width)) {
    delete[] rgbs;
    throw std::runtime_error("Failed to write to PNG file");
  }
  delete[] rgbs;
}

void write_rgba_image(const std::string &filename, const math::Tensor<math::TColor<float, 4>, 2> &data) {
  if (!stbi_write_png(filename.c_str(), data.shape()[1], data.shape()[0], 4, data.raw_data(), 0)) {
    throw std::runtime_error("Failed to write to PNG file");
  }
}

}  // namespace misaki::image