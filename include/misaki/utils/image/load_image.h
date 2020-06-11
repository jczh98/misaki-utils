#pragma once

#include "../math.h"

namespace misaki::image {

TColor<float, 3> load_image(const std::string &filename);

}