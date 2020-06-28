#pragma once

#include "scalar.h"
#include "vector.h"

namespace misaki::math {

template <typename Float>
struct DiscreteDistribution {
  DiscreteDistribution() {}
  DiscreteDistribution(const std::vector<Float> &pmf) : m_pmf(pmf) {
    update();
  }
  void update() {
    size_t size = m_pmf.size();
    if (size == 0) {
      throw std::runtime_error("DiscreteDistribution: empty distribution");
    }
    if (m_cdf.size() != size) m_cdf.resize(size);
    m_valid = TVector<uint32_t, 2>(-1, -1);
    double sum = 0.0;
    for (uint32_t i = 0; i < size; i++) {
      double value = m_pmf[i];
      sum += value;
      m_cdf[i] = sum;
      if (value < 0.0) {
        throw std::runtime_error("DiscreteDistribution: entries must be non-negative");
      } else if (value > 0.0) {
        if (m_valid.x() == -1) m_valid.x() = i;
        m_valid.y() = i;
      }
    }
    if ((m_valid.array() == -1).any()) {
      throw std::runtime_error("DiscreteDistribution: no probability mass found");
    }
    m_sum = sum;
    m_normalization = 1.0 / sum;
  }

  uint32_t sample(Float value) const {
    value *= m_sum;
    const auto it = std::upper_bound(m_cdf.begin(), m_cdf.end(), value);
    return std::clamp(uint32_t(std::distance(m_cdf.begin(), it)) - 1, m_valid.x(), m_valid.y() + 1);
  }

  Float eval_pmf(uint32_t index) const {
    return m_pmf[index];
  }

  Float eval_pmf_normalized(uint32_t index) const {
    return m_pmf[index] * m_normalization;
  }

  Float eval_cdf(uint32_t index) const {
    return m_cdf[index];
  }

  Float eval_cdf_normalized(uint32_t index) const {
    return m_cdf[index] * m_normalization;
  }

  std::pair<uint32_t, Float> sample_reuse(Float value) const {
    uint32_t index = sample(value);
    Float pmf = eval_pmf_normalized(index);
    Float cdf = index > 0 ? eval_cdf_normalized(index - 1) : 0;
    return {index, (value - cdf) / pmf};
  }

  Float sum() const { return m_sum; }
  Float normalization() const { return m_normalization; }

  bool empty() const { return m_pmf.empty(); }

  std::vector<Float> m_pmf;
  std::vector<Float> m_cdf;
  Float m_sum = 0.f;
  Float m_normalization = 0.f;
  TVector<uint32_t, 2> m_valid;
};

}  // namespace misaki::math