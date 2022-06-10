#ifndef NONBIRI_CORE_HTTP_CLIENT_H_
#define NONBIRI_CORE_HTTP_CLIENT_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <core/http/interceptor.h>
#include <core/http/ratelimiter.h>
#include <core/http/types.h>

namespace Http
{
struct Client
{
  RateLimiter *rateLimiter {nullptr};
  std::map<std::string, std::string> defaultHeaders;
  std::vector<std::shared_ptr<Interceptor>> interceptors;

public:
  Client(RateLimiter *rateLimiter = nullptr);
  ~Client();

  std::shared_ptr<Response> get(const std::string &url, const Headers &headers = {}) const;
  std::shared_ptr<Response> post(const std::string &url, const std::string &body, const Headers &headers = {}) const;
  std::shared_ptr<Response> send(Request &request) const;
  long download(const std::string &url, const std::string &path) const;

  void setRateLimiter(RateLimiter *rateLimiter);
  void setDefaultHeader(const std::string &name, const std::string &value);
  void removeDefaultHeader(const std::string &name);
  void addInterceptor(std::shared_ptr<Interceptor> interceptor);
  void addInterceptor(Interceptor *interceptor);
};
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_CLIENT_H_