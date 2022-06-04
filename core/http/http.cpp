#include <core/http/http.h>

namespace Http
{
std::shared_ptr<Response> get(const std::string &url, const Headers &headers)
{
  return Client().get(url, headers);
}

std::shared_ptr<Response> post(const std::string &url, const std::string &body, const Headers &headers)
{
  return Client().post(url, body, headers);
}

std::shared_ptr<Response> send(const Request &request)
{
  return Client().send(request);
}

int download(const std::string &url, const std::string &path)
{
  return Client().download(url, path);
}
};  // namespace Http