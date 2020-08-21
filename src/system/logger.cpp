#include <misaki/utils/system/logger.h>
#include <stdio.h>

#include <ctime>
#include <filesystem>
#include <mutex>
#include <sstream>

namespace misaki::system {

LogConfig GLOBAL_LOGCONFIG;

void init_logging(LogConfig config, bool use_gpu) {
  GLOBAL_LOGCONFIG = config;
}

void Log(LogLevel level, const char *file, int line, const char *msg) {
  std::ostringstream oss;
  std::istringstream iss(msg);
  char buffer[128];
  std::string msg_line;
  time_t time_ = std::time(nullptr);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", std::localtime(&time_));
  int line_idx = 0;

  while (std::getline(iss, msg_line) || line_idx == 0) {
    if (line_idx > 0)
      oss << '\n';
    oss << buffer;

    switch (level) {
      case LogLevel::Verbose:
        oss << "VERBOSE ";
        break;
      case LogLevel::Debug:
        oss << "DEBUG ";
        break;
      case LogLevel::Info:
        oss << "INFO  ";
        break;
      case LogLevel::Warn:
        oss << "WARN  ";
        break;
      case LogLevel::Error:
        oss << "ERROR ";
        break;
      default:
        oss << "CUSTM ";
        break;
    }

    if (line != -1 && file)
      oss << "[" << std::filesystem::path(file).filename() << ":" << line << "] ";

    oss << msg_line;
    line_idx++;
  }
  oss << '\n';
  fmt::print(oss.str());
}

void LogFatal(const char *file, int line, const char *msg) {
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock(mutex);
  std::ostringstream oss;
  std::istringstream iss(msg);
  char buffer[128];
  std::string msg_line;
  time_t time_ = std::time(nullptr);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", std::localtime(&time_));
  int line_idx = 0;

  while (std::getline(iss, msg_line) || line_idx == 0) {
    if (line_idx > 0)
      oss << '\n';
    oss << buffer << "FATAL ";

    if (line != -1 && file)
      oss << "[" << std::filesystem::path(file).filename() << ":" << line << "] ";

    oss << msg_line;
    line_idx++;
  }
  oss << '\n';
  fmt::print(oss.str());
  abort();
}

}  // namespace misaki::system