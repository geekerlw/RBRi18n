#pragma once

#ifdef _DEBUG
#define DebugLog(msg) LogUtil::ToFile(msg)
#else
// In Release build DebugLog does nothing
#define DebugLog(msg) ((void)0)
#endif

// For now InfoLog and ErrorLog does the same thing
#define InfoLog(msg)  LogUtil::ToFile(msg)
#define ErrorLog(msg) LogUtil::ToFile(msg)

#define FormatLog(msg, ...) \
do { \
  std::string logbuf(256, '\0');\
  int len = std::snprintf(logbuf.data(), logbuf.size(), msg, ##__VA_ARGS__); \
  logbuf.resize(len); \
  LogUtil::ToFile(logbuf); \
} while(0)

namespace LogUtil {
  using std::wstring;
  using std::string;

  void SetLogFile(const char* file);

  void ToFile(const wstring& message);
  void ToScreen(const wstring& message);
  
  void ToFile(const string& message);
  void ToScreen(const string& message);
  
  void ToFile(const char* message);
  void ToScreen(const char* message);

  void LastExceptionToFile(const string& source);
  std::string ErrNoToString(int errorno);
}

