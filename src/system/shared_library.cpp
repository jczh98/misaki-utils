#include <misaki/utils/system/shared_library.h>

#include <stdexcept>

namespace misaki::system {

SharedLibrary::SharedLibrary(const std::string &filename) : path(filename) {
#if defined(MSK_PLATFORM_WINDOWS)
  handle = LoadLibraryA(filename.c_str());
  if (!handle) throw std::runtime_error("Error while loading shared library: " + filename);
#endif
}
SharedLibrary::~SharedLibrary() {
#if defined(MSK_PLATFORM_WINDOWS)
  FreeLibrary(handle);
#endif
}
SharedSymbolPtr SharedLibrary::load_symbol(const std::string &name) {
#if defined(MSK_PLATFORM_WINDOWS)
  void *ptr = GetProcAddress(handle, name.c_str());
  if (!ptr) throw std::runtime_error("Could not resolve symbol \"" + name + "\" in \"" + path + "\"");
#endif
  return ptr;
}

}  // namespace misaki::system