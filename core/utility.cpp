#include <core/utility.h>

void *Utils::loadLibrary(const std::string &path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return LoadLibrary(path.c_str());
#else
  return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void *Utils::getSymbol(void *handle, const std::string &symbol)
{
  if (handle == nullptr)
    return nullptr;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return GetProcAddress((HMODULE)handle, symbol.c_str());
#else
  return dlsym(handle, symbol.c_str());
#endif
}

void Utils::freeLibrary(void *handle)
{
  if (handle == nullptr)
    return;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  FreeLibrary((HMODULE)handle);
#else
  dlclose(handle);
#endif
}

Mutex::Mutex()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  mutex = CreateMutex(nullptr, false, nullptr);
#else
  pthread_mutex_init(&mutex, nullptr);
#endif
}

Mutex::~Mutex()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  CloseHandle(mutex);
#else
  pthread_mutex_destroy(&mutex);
#endif
}

void Mutex::lock()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  WaitForSingleObject(mutex, INFINITE);
#else
  pthread_mutex_lock(&mutex);
#endif
}

void Mutex::unlock()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  ReleaseMutex(mutex);
#else
  pthread_mutex_unlock(&mutex);
#endif
}

MutexLock::MutexLock(Mutex &mutex) : mutex(mutex)
{
  mutex.lock();
}

MutexLock::~MutexLock()
{
  mutex.unlock();
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
