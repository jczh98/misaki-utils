#pragma once

#include "../container.hpp"
#include "../math.hpp"
#include "../stl.hpp"

namespace misaki::image {
// PixelFormat Definition
enum class PixelFormat { UInt,
                         Float };

// PixelFormat Inline Functions
MSK_CPU_GPU
inline bool is_8bit(PixelFormat format) {
  return format == PixelFormat::UInt;
}
MSK_CPU_GPU
inline bool is_32bit(PixelFormat format) {
  return format == PixelFormat::Float;
}

std::string to_string(PixelFormat format);

struct ImageMetadata {
  mstd::optional<math::Vector2i> full_resolution;
  std::string to_string() const;
};

struct ImageChannelDesc {
  operator bool() const { return size() > 0; }

  size_t size() const { return offset.size(); }
  bool is_identity() const {
    for (size_t i = 0; i < offset.size(); ++i)
      if (offset[i] != i)
        return false;
    return true;
  }
  std::string to_string() const;

  container::InlinedVector<int, 4> offset;
};

// ImageChannelValues Definition
template <typename Float>
struct ImageChannelValues : public container::InlinedVector<Float, 4> {
  // ImageChannelValues() = default;
  explicit ImageChannelValues(size_t sz, Float v = {})
      : container::InlinedVector<Float, 4>(sz, v) {}

  operator Float() const {
    CHECK_EQ(1, size());
    return (*this)[0];
  }
  operator mstd::array<Float, 3>() const {
    CHECK_EQ(3, size());
    return {(*this)[0], (*this)[1], (*this)[2]};
  }

  Float max_value() const {
    Float m = (*this)[0];
    for (int i = 1; i < size(); ++i)
      m = std::max(m, (*this)[i]);
    return m;
  }
  Float average() const {
    Float sum = 0;
    for (int i = 0; i < size(); ++i)
      sum += (*this)[i];
    return sum / size();
  }

  std::string to_string() const;
};

class Image {
 public:
  using Allocator = mstd::pmr::polymorphic_allocator<std::byte>;
  Image(Allocator alloc = {})
      : m_p8(alloc),
        m_p32(alloc),
        m_format(PixelFormat::UInt),
        m_resolution(0, 0) {}
  Image(mstd::vector<float> p32, math::Vector2i resolution,
        mstd::span<const std::string> channels);

 private:
  PixelFormat m_format;
  math::Vector2i m_resolution;
  container::InlinedVector<std::string, 4> m_channel_names;
  mstd::vector<uint8_t> m_p8;
  mstd::vector<float> m_p32;
};

}  // namespace misaki::image