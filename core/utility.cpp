#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif
#include <core/utility.h>

namespace Utils
{
void *loadLibrary(const std::string &path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return LoadLibrary(path.c_str());
#else
  return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void *getSymbol(void *handle, const std::string &symbol)
{
  if (handle == nullptr)
    return nullptr;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return GetProcAddress((HMODULE)handle, symbol.c_str());
#else
  return dlsym(handle, symbol.c_str());
#endif
}

void freeLibrary(void *handle)
{
  if (handle == nullptr)
    return;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  FreeLibrary((HMODULE)handle);
#else
  dlclose(handle);
#endif
}

std::string stripPath(const std::string &url)
{
  if (url.find("://") == std::string::npos)
    return url;
  return url.substr(0, url.find("/", 8));
}

std::string stripDomain(const std::string &url)
{
  if (url.find("://") == std::string::npos)
    return url;
  size_t pos = url.find("/", 8);
  if (pos == std::string::npos)
    return "/";
  return url.substr(pos);
}

std::string getFilename(const std::string &path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define SEPARATOR "\\"
#else
#  define SEPARATOR "/"
#endif
  auto pos = path.find_last_of(SEPARATOR);
  if (pos == std::string::npos)
    return path;
  return path.substr(pos + 1);
}

bool strcmpi(const std::string &str1, const std::string &str2)
{
  if (str1.size() != str2.size())
    return false;
  for (size_t i = 0; i < str1.size(); i++) {
    if (tolower(str1[i]) != tolower(str2[i]))
      return false;
  }
  return true;
}
}  // namespace Utils
