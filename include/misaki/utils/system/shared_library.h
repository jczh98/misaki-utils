#pragma once

#include "platform.h"
#include <string>
#if MSK_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace misaki::system {

using SharedSymbolPtr = void*;

class SharedLibrary {
 public:
  SharedLibrary(const std::string &filename);
  ~SharedLibrary();
  SharedSymbolPtr load_symbol(const std::string &func_name);
 private:
  std::string path;
#if MSK_PLATFORM_WINDOWS
  HMODULE handle;
#else
  void *handle;
#endif
};

}