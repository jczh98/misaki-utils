#pragma once

#include <fmt/format.h>

#include "platform.h"

namespace misaki::system {

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

MSK_CPU_GPU void Log(LogLevel level, const char *file, int line, const char *s);

MSK_CPU_GPU [[noreturn]] void LogFatal(const char *file, int line, const char *s);

template <typename... Args>
MSK_CPU_GPU inline void Log(LogLevel level, const char *file, int line, const char *fmt,
                            Args &&... args) {
  Log(level, file, line, fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
MSK_CPU_GPU [[noreturn]] inline void LogFatal(const char *file, int line,
                                              const char *fmt, Args &&... args) {
  LogFatal(level, file, line, fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

#ifdef MSK_IS_GPU_CODE

#else

#define LogDebug(...)                                           \
  (misaki::system::LogLevel::Debug >= GLOBAL_LOGCONFIG.level && \
   (misaki::system::Log(misaki::system::LogLevel::Debug, __FILE__, __LINE__, __VA_ARGS__), true))

#define LogInfo(...)                                           \
  (misaki::system::LogLevel::Info >= GLOBAL_LOGCONFIG.level && \
   (misaki::system::Log(misaki::system::LogLevel::Info, __FILE__, __LINE__, __VA_ARGS__), true))

#define LogWarn(...)                                           \
  (misaki::system::LogLevel::Warn >= GLOBAL_LOGCONFIG.level && \
   (misaki::system::Log(misaki::system::LogLevel::Warn, __FILE__, __LINE__, __VA_ARGS__), true))

#define LogError(...)                                           \
  (misaki::system::LogLevel::Error >= GLOBAL_LOGCONFIG.level && \
   (misaki::system::Log(misaki::system::LogLevel::Error, __FILE__, __LINE__, __VA_ARGS__), true))

#define Fatal(...) misaki::system::LogFatal(__FILE__, __LINE__, __VA_ARGS__)

#endif

}  // namespace misaki