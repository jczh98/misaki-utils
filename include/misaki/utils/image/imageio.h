#pragma once

#include "../math.h"

namespace misaki::image {

math::Tensor<math::TColor<float, 3>, 2> read_rgb_image(const std::string &filename);

math::Tensor<math::TColor<float, 4>, 2> read_rgba_image(const std::string &filename);

void write_rgb_image(const std::string &filename, const math::Tensor<math::TColor<float, 3>, 2> &data);

void write_rgba_image(const std::string &filename, const math::Tensor<math::TColor<float, 4>, 2> &data);

}