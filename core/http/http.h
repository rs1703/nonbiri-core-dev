#ifndef NONBIRI_CORE_HTTP_H_
#define NONBIRI_CORE_HTTP_H_

#include <memory>
#include <string>

#include <core/http/client.h>
#include <core/http/interceptor.h>
#include <core/http/ratelimiter.h>
#include <core/http/types.h>

namespace Http
{
std::shared_ptr<Response> get(const std::string &url, const Headers &headers = {});
std::shared_ptr<Response> post(const std::string &url, const std::string &body, const Headers &headers = {});
std::shared_ptr<Response> send(Request &request);
int download(const std::string &url, const std::string &path);
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_H_