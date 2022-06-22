#ifndef NONBIRI_CORE_HTTP_STRUCT_H_
#define NONBIRI_CORE_HTTP_STRUCT_H_

#include <map>
#include <string>

#include <core/http/header.h>

namespace Http
{
struct Request
{
  std::string url {};
  std::string method {};
  std::string body {};
  Headers headers {};
  bool followRedirects {};
};

struct Response
{
  const std::string url {};
  const std::string method {};
  const std::string body {};
  const Headers headers {};
  const long statusCode {};
};
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_STRUCT_H_