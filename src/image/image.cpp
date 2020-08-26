#include <misaki/utils/util.h>

#include <misaki/utils/image/image.hpp>

namespace misaki::image {

std::string to_string(PixelFormat format) {
  switch (format) {
    case PixelFormat::UInt:
      return "UInt8";
    case PixelFormat::Float:
      return "Float";
    default:
      Fatal("Unhandled PixelFormat in FormatName()");
      return "";
  }
}

template <typename Float>
std::string ImageChannelValues<Float>::to_string() const {
  return fmt::format("[ ImageChannelValues {} ]", ((container::InlinedVector<Float, 4> &)*this));
}

std::string ImageChannelDesc::to_string() const {
  return fmt::format("[ ImageChannelDesc offset: {} ]", offset);
}

std::string ImageMetadata::to_string() const {
  return fmt::format(
      "[ ImageMetadata ]");
}

Image::Image(mstd::vector<float> p32c, math::Vector2i resolution,
             mstd::span<const std::string> channels)
    : m_format(PixelFormat::Float),
      m_resolution(resolution),
      m_channel_names(channels.begin(), channels.end()),
      m_p32(std::move(p32c)) {
}

}  // namespace misaki::image