#include <stdexcept>

#include <core/core.h>
#include <core/http/client.h>
#include <string.h>

size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  std::string &body = *static_cast<std::string *>(userp);
  body.append(static_cast<char *>(contents), size * nmemb);
  return size * nmemb;
}

namespace Http
{
Client::Client(RateLimiter *rateLimiter) : rateLimiter {rateLimiter} {}
Client::~Client()
{
  if (rateLimiter != nullptr)
    delete rateLimiter;
}

std::shared_ptr<Response> Client::get(const std::string &url, const Headers &headers) const
{
  Request request {url, "GET", "", headers};
  return send(request);
}

std::shared_ptr<Response> Client::post(const std::string &url, const std::string &body, const Headers &headers) const
{
  Request request {url, "POST", body, headers};
  return send(request);
}

std::shared_ptr<Response> Client::send(const Request &request) const
{
  CURL *curl = init();
  if (curl == nullptr)
    return nullptr;

  const char *method = request.method.c_str();
  setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  setOpt(curl, CURLOPT_URL, request.url.c_str());
  setOpt(curl, CURLOPT_CUSTOMREQUEST, method);

  if (strcmpi(method, "POST") == 0 && !request.body.empty())
    setOpt(curl, CURLOPT_POSTFIELDS, request.body.c_str());

  for (const auto &header : request.headers)
    setOpt(curl, CURLOPT_HTTPHEADER, header.name.c_str(), header.value.c_str());

  std::string body {};
  setOpt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  setOpt(curl, CURLOPT_WRITEDATA, &body);

  if (rateLimiter != nullptr)
    rateLimiter->acquire();

  auto code = perform(curl);
  if (code != CURLE_OK) {
    cleanup(curl);
    throw std::runtime_error(std::string("Failed to send request: ") + getError(code));
  }

  long statusCode {};

  getInfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
  cleanup(curl);

  return std::make_shared<Response>(body, statusCode);
}

int Client::download(const std::string &url, const std::string &path) const
{
  CURL *curl = init();
  if (curl == nullptr)
    return -1;

  setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0);

  FILE *fp = fopen(path.c_str(), "wb");
  if (fp == nullptr) {
    cleanup(curl);
    return -1;
  }

  setOpt(curl, CURLOPT_URL, url.c_str());
  setOpt(curl, CURLOPT_WRITEDATA, fp);
  setOpt(curl, CURLOPT_WRITEFUNCTION, NULL);

  if (rateLimiter != nullptr)
    rateLimiter->acquire();

  const auto code = perform(curl);
  long statusCode {};

  if (code == CURLE_OK)
    getInfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

  cleanup(curl);
  fclose(fp);

  return static_cast<int>(statusCode);
}
}  // namespace Http