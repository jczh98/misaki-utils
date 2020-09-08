#pragma once

#include <fmt/format.h>

#include "../system.h"

namespace misaki::util {

enum class LogLevel { Verbose,
                      Debug,
                      Info,
                      Warn,
                      Error,
                      Fatal };

struct LogConfig {
  LogLevel level = LogLevel::Debug;
};

void init_logging(LogConfig config, bool use_gpu = false);

#ifdef MSK_BUILD_ON_GPU
struct GPULogItem {
  LogLevel level;
  char file[64];
  int line;
  char message[128];
};
std::vector<GPULogItem> read_gpu_logs();
#endif

extern LogConfig GLOBAL_LOGCONFIG;

MSK_XPU void Log(LogLevel level, const char *file, int line, const char *s);

MSK_XPU [[noreturn]] void LogFatal(const char *file, int line, const char *s);

template <typename... Args>
MSK_XPU inline void Log(LogLevel level, const char *file, int line, const char *fmt,
                            Args &&... args) {
  Log(level, file, line, fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
MSK_XPU [[noreturn]] inline void LogFatal(const char *file, int line,
                                              const char *fmt, Args &&... args) {
  LogFatal(file, line, fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

#ifdef MSK_IS_GPU_CODE

#else

#define LogDebug(...)                                            \
  (misaki::util::LogLevel::Debug >= misaki::util::GLOBAL_LOGCONFIG.level && \
   (misaki::util::Log(misaki::util::LogLevel::Debug, __FILE__, __LINE__, __VA_ARGS__), true))

#define LogInfo(...)                                                             \
  (misaki::util::LogLevel::Info >= misaki::util::GLOBAL_LOGCONFIG.level && \
   (misaki::util::Log(misaki::util::LogLevel::Info, __FILE__, __LINE__, __VA_ARGS__), true))

#define LogWarn(...)                                                             \
  (misaki::util::LogLevel::Warn >= misaki::util::GLOBAL_LOGCONFIG.level && \
   (misaki::util::Log(misaki::util::LogLevel::Warn, __FILE__, __LINE__, __VA_ARGS__), true))

#define LogError(...)                                            \
  (misaki::util::LogLevel::Error >= misaki::util::GLOBAL_LOGCONFIG.level && \
   (misaki::util::Log(misaki::util::LogLevel::Error, __FILE__, __LINE__, __VA_ARGS__), true))

#define Fatal(...) misaki::util::LogFatal(__FILE__, __LINE__, __VA_ARGS__)

#endif

}  // namespace misaki::util