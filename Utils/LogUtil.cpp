#include <Windows.h>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>
#include "LogUtil.h"

namespace LogUtil {

using std::wstring;
using std::string;

std::wofstream output;
static std::mutex g_logMutex;

static std::wstring ToWide(const std::string& s)
{
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    if (n <= 0) return L"";
    std::wstring r(n, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &r[0], n);
    return r;
}

static std::string GetTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;
    struct tm tm_info;
    localtime_s(&tm_info, &time_t);
    ss << std::put_time(&tm_info, "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void SetLogFile(const char* file)
{
    output.close();
    output.open(file);
}

void ToFile(const wstring& message)
{
    std::lock_guard<std::mutex> lock(g_logMutex);
    if (!output.is_open()) return;
    output << L"[" << ToWide(GetTimestamp()) << L"] " << message << std::endl;
    output.flush();
}

void ToScreen(const wstring& message) { ToFile(L"To Screen: " + message); }
void ToFile(const string& message) { ToFile(ToWide(message)); }
void ToScreen(const string& message) { ToScreen(ToWide(message)); }
void ToFile(const char* message) { ToFile(string(message)); }
void ToScreen(const char* message) { ToScreen(string(message)); }

void LastExceptionToFile(const string& source) {
    try { std::rethrow_exception(std::current_exception()); }
    catch (const std::exception& e) { ToFile(source + " Exception: " + e.what()); }
    catch (...) { ToFile(source + " Exception: Unknown"); }
}

std::string ErrNoToString(int errorno) {
    char buf[256] = { 0 };
    strerror_s(buf, errorno);
    return std::string(buf) + " (" + std::to_string(errorno) + ")";
}

}
