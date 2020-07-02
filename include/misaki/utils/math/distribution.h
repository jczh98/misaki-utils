#pragma once

#include <algorithm>
#include <numeric>

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
    return math::binary_search(m_valid.x(), m_valid.y() + 1,
                               [&](uint32_t index) {
                                 return m_cdf[index] < value;
                               });
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

template <typename Float>
struct Distribution1D {
  std::vector<Float> m_cdf{0};
  void init(const Float *data, int n) {
    m_cdf.clear();
    m_cdf.push_back(0.f);
    std::partial_sum(data, data + n, std::back_inserter(m_cdf));
    const Float inv_sum = 1.f / m_cdf.back();
    for (auto &cdf : m_cdf) cdf *= inv_sum;
  }

  const std::vector<Float> &cdf() const { return m_cdf; }

  Float pmf(int i) const {
    return (i < 0 || i + 1 >= int(m_cdf.size())) ? 0 : m_cdf[i + 1] - m_cdf[i];
  }

  uint32_t sample(Float u) const {
    const auto it = std::upper_bound(m_cdf.begin(), m_cdf.end(), u);
    return std::clamp(int(std::distance(m_cdf. begin(), it)) - 1, 0, int(m_cdf.size()) - 2);
  }

  std::pair<uint32_t, Float> sample_reuse(Float u) const {
    uint32_t index = sample(u);
    return {index, (u - m_cdf[index]) / pmf(index)};
  }
};

template <typename Float>
struct Distribution2D {
  std::vector<Distribution1D<Float>> m_conditionl;
  Distribution1D<Float> m_marginal;
  int w, h;
  void init(const Float *data, int cols, int rows) {
    w = cols;
    h = rows;
    m_conditional.assign(h, {});
    std::vector<Float> m;
    for (int i = 0; i < h; ++i) {
      auto &d = m_conditional[i];
      d.init(data[i * w], w);
      m.push_back(d.cdf().back());
    }
    m_marginal.init(m.data(), m.size());
  }

  Float pdf(Float u, Float v) const {
    const int y = std::min(int(v * h), h - 1);
    return m_marginal.pmf(y) * m_conditionl[y].pmf(int(u * w)) * w * h;
  }

  std::pair<Float, Float> sample(const math::TVector<Float, 4> &u) const {
    const int y = m_marginal.sample(u[0]);
    const int x = m_conditionl[y].sample(u[1]);
    return {(x + u[2]) / w, (y + u[3]) / h};
  }
};

}  // namespace misaki::math