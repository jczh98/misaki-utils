#pragma once

#include <chrono>

namespace misaki::util {
class Timer {
 public:
  Timer() {
    start = std::chrono::system_clock::now();
  }

  size_t value() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return (size_t)duration.count();
  }

  size_t reset() {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    start = now;
    return (size_t)duration.count();
  }

 private:
  std::chrono::system_clock::time_point start;
};

}  // namespace misaki::util