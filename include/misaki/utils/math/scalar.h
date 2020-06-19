namespace misaki::math {

template <typename T>
T clamp(T value, T vmin, T vmax) {
  return value < vmin ? vmin : (value > vmax ? vmax : value);
}

}  // namespace misaki::math