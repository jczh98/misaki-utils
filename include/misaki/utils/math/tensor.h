#pragma once

#include "vector.h"

namespace misaki::math {

template <typename Value, int Dimension>
class Tensor {
 public:
  using Index = TVector<int, Dimension>;
  using Self = Tensor<Value, Dimension>;

  Tensor() : m_data(nullptr), m_shape(0), m_data_size(0) {}
  Tensor(const Index &index) : m_data(nullptr), m_shape(index), m_data_size(hprod(index)) {
    m_data = std::unique_ptr<Value[]>(new Value[m_data_size]);
  }
  Tensor(const Self &copy_from) : Tensor() {
    if (copy_from.m_data != nullptr) {
      std::copy(copy_from.m_data.get(), copy_from.m_data.get() + copy_from.m_data_size, m_data.get());
      m_shape = copy_from.m_shape;
      m_data_size = copy_from.m_data_size;
    }
  }
  Tensor(Self &&move_from) noexcept : Tensor() {
    m_data = std::move(move_from.m_data);
    m_shape = move_from.m_shape;
    m_data_size = move_from.m_data_size;
  }

  static Self from_linear_indexed(const Index &shape, const std::function<Value(int)> &func) {
    Tensor<Value, Dimension> result(shape);
    Value *data = new Value[result.data_size()];
    for (int i = 0; i < result.data_size(); ++i) {
      new (&data[i]) Value(func(i));
    }
    result.m_data.reset(data);
    return result;
  }

  Self &operator=(const Self &copy_from) {
    if (m_shape != copy_from.m_shape) {
      std::copy(copy_from.m_data.get(), copy_from.m_data.get() + copy_from.m_data_size, m_data.get());
      m_shape = copy_from.m_shape;
      m_data_size = copy_from.m_data_size;
      return *this;
    }
    for (int i = 0; i < m_data_size; ++i) m_data[i] = copy_from.m_data[i];
    return *this;
  }
  Self &operator=(Self &&move_from) noexcept {
    m_data = std::move(move_from.m_data);
    m_shape = move_from.m_shape;
    m_data_size = move_from.m_data_size;
    return *this;
  }
  ~Tensor() {
    m_shape = Index(0);
    m_data_size = 0;
  }

  const Index &shape() const noexcept { return m_shape; }
  int data_size() const noexcept { return m_data_size; }
  Value *raw_data() noexcept { return m_data.get(); }
  const Value *raw_data() const noexcept { return m_data.get(); }

 private:
  std::unique_ptr<Value[]> m_data;
  Index m_shape;
  int m_data_size;
};

}  // namespace misaki::math