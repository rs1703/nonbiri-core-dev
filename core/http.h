#ifndef NONBIRI_CORE_HTTP_H_
#define NONBIRI_CORE_HTTP_H_

#include <chrono>
#include <string>

#include <core/utility.h>
#include <curl/curl.h>

namespace Http
{
typedef CURL *(*init_t)(void);
typedef CURLcode (*setOpt_t)(CURL *, CURLoption, ...);
typedef CURLcode (*perform_t)(CURL *handle);
typedef void (*cleanup_t)(CURL *handle);
typedef CURLcode (*getInfo_t)(CURL *handle, CURLINFO info, ...);

extern Http::init_t init;
extern Http::setOpt_t setOpt;
extern Http::perform_t perform;
extern Http::cleanup_t cleanup;
extern Http::getInfo_t getInfo;

typedef void (*initialize_t)(
  init_t initPtr, setOpt_t setOptPtr, perform_t performPtr, cleanup_t cleanupPtr, getInfo_t getInfoPtr);
EXPORT_API void initialize(
  init_t initPtr, setOpt_t setOptPtr, perform_t performPtr, cleanup_t cleanupPtr, getInfo_t getInfoPtr);
std::string get(const std::string &url);
int download(const std::string &url, const std::string &path);
};  // namespace Http

class RateLimiter
{
  int64_t tokens {};
  const int64_t size {};
  const std::chrono::seconds interval {};
  std::chrono::time_point<std::chrono::system_clock> lastTime {};
  Mutex mutex {};

public:
  RateLimiter(int64_t size, std::chrono::seconds interval = std::chrono::seconds(1));
  void acquire();
};

class Manager;
class Api;

class HttpClient
{
  friend class Manager;
  friend class Api;

  RateLimiter *rateLimiter {nullptr};

public:
  HttpClient(RateLimiter *rateLimiter = nullptr);
  ~HttpClient();

  std::string get(const std::string &url) const;

private:
  int download(const std::string &url, const std::string &path) const;
};

#endif  // NONBIRI_CORE_HTTP_H_