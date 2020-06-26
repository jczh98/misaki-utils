#pragma once

#include "../system.h"

namespace misaki::util {

extern std::string time_string(float time, bool precise = false);
extern std::string mem_string(size_t size, bool precise = false);

}