#ifndef NONBIRI_CORE_UTILITY_H_
#define NONBIRI_CORE_UTILITY_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define EXPORT_API extern "C" __declspec(dllexport)
#else
#  define EXPORT_API extern "C"
#endif
#include <string>

namespace Utils
{
void *loadLibrary(const std::string &path);
void *getSymbol(void *handle, const std::string &symbol);
void freeLibrary(void *handle);

std::string stripPath(const std::string &url);
std::string stripDomain(const std::string &url);
std::string getFilename(const std::string &path);
};  // namespace Utils

#endif  // NONBIRI_CORE_UTILITY_H_