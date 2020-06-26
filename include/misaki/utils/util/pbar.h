#pragma once

#include <atomic>
#include <iostream>
#include <mutex>

namespace misaki::util {

class ProgressBar {
 public:
  ProgressBar(size_t tot, size_t width) : m_total(tot), m_width(width) { show(); }
  void update() {
    m_count++;
    show();
  }
  void done() { std::cout << std::endl; }

 private:
  void show() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << "[";
    float progress = float(m_count) / m_total;
    size_t pos = size_t(progress * m_width);
    for (size_t i = 0; i < m_width; ++i) {
      if (i < pos)
        std::cout << "=";
      else if (i == pos)
        std::cout << ">";
      else
        std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << "% (" << m_count << "/" << m_total << ")"
              << " \r";
    std::cout.flush();
  }
  std::atomic<size_t> m_count = 0;
  std::mutex m_mutex;
  size_t m_total, m_width;
};

}