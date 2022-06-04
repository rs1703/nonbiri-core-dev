#ifndef NONBIRI_CORE_HTTP_STRUCT_H_
#define NONBIRI_CORE_HTTP_STRUCT_H_

#include <string>
#include <vector>

namespace Http
{
struct Header
{
  std::string name {};
  std::string value {};
};

typedef std::vector<Header> Headers;

struct Request
{
  std::string url {};
  std::string method {};
  std::string body {};
  Headers headers {};
};

struct Response
{
  std::string body {};
  long statusCode {};

  Response(const std::string &body, long statusCode) : body {body}, statusCode {statusCode} {}
  Response(long statusCode, const std::string &body) : body {body}, statusCode {statusCode} {}
};
};  // namespace Http

#endif  // NONBIRI_CORE_HTTP_STRUCT_H_