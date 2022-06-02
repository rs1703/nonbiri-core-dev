#ifndef NONBIRI_CORE_UTILITY_H_
#define NONBIRI_CORE_UTILITY_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  include <windows.h>
#  define EXPORT_API extern "C" __declspec(dllexport)
#else
#  include <dlfcn.h>
#  include <pthread.h>
#  define EXPORT_API extern "C"
#endif

#include <atomic>
#include <string>

namespace Utils
{
void *loadLibrary(const std::string &path);
void *getSymbol(void *handle, const std::string &symbol);
void freeLibrary(void *handle);
};  // namespace Utils

class Mutex
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  HANDLE mutex;
#else
  pthread_mutex_t mutex;
#endif

public:
  Mutex();
  ~Mutex();

  void lock();
  void unlock();
};

class MutexLock
{
  Mutex &mutex;

public:
  MutexLock(Mutex &mutex);
  ~MutexLock();
};

std::string stripPath(const std::string &url);
std::string stripDomain(const std::string &url);
std::string getFilename(const std::string &path);

#endif  // NONBIRI_CORE_UTILITY_H_