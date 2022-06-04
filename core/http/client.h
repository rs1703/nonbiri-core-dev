#ifndef NONBIRI_CORE_HTTP_CLIENT_H_
#define NONBIRI_CORE_HTTP_CLIENT_H_

#include <memory>
#include <string>

#include <core/http/ratelimiter.h>
#include <core/http/types.h>

namespace Http
{
struct Client
{
  RateLimiter *rateLimiter {nullptr};
  Client(RateLimiter *rateLimiter = nullptr);
  ~Client();

  std::shared_ptr<Response> get(const std::string &url, const Headers &headers = {}) const;
  std::shared_ptr<Response> post(const std::string &url, const std::string &body, const Headers &headers = {}) const;
  std::shared_ptr<Response> send(const Request &request) const;
  int download(const std::string &url, const std::string &path) const;
};
};  // namespace Http

#endif  // NONBIRI_CORE_HTTP_CLIENT_H_