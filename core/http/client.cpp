#include <filesystem>
#include <stdexcept>

#include <core/core.h>
#include <core/http/client.h>
#include <core/utility.h>

namespace fs = std::filesystem;

static const std::string userAgent {
  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 "
  "Safari/537.36",
};

size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  std::string &body = *static_cast<std::string *>(userp);
  body.append(static_cast<char *>(contents), size * nmemb);
  return size * nmemb;
}

size_t headerCallback(void *contents, size_t size, size_t nmemb, Http::Headers *headers)
{
  const size_t ret = size * nmemb;
  const std::string header = std::string(static_cast<char *>(contents), ret - 2);
  if (!header.empty()) {
    const size_t colon = header.find(':');
    if (colon != std::string::npos)
      headers->set(header.substr(0, colon), header.substr(colon + 2));
  }
  return ret;
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
  Interceptor::Chain chain {*this, request};
  for (auto &interceptor : interceptors)
    interceptor->intercept(chain);
  return chain.proceed();
}

std::shared_ptr<Response> Client::post(const std::string &url, const std::string &body, const Headers &headers) const
{
  Request request {url, "POST", body, headers};
  Interceptor::Chain chain {*this, request};
  for (auto &interceptor : interceptors)
    interceptor->intercept(chain);
  return chain.proceed();
}

std::shared_ptr<Response> Client::send(Request &request) const
{
  CURL *curl = init();
  if (curl == nullptr)
    throw std::runtime_error("Failed to initialize curl");

  Headers responseHeaders {};
  std::string body {};

  setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  setOpt(curl, CURLOPT_URL, request.url.c_str());
  setOpt(curl, CURLOPT_CUSTOMREQUEST, request.method.c_str());
  setOpt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
  setOpt(curl, CURLOPT_HEADERDATA, &responseHeaders);
  setOpt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  setOpt(curl, CURLOPT_WRITEDATA, &body);

  if (Utils::strcmpi(request.method, "POST") && !request.body.empty())
    setOpt(curl, CURLOPT_POSTFIELDS, request.body.c_str());

  if (cookies != nullptr)
    cookies->build(request.url, curl);

  Headers requestHeaders {headers};
  requestHeaders.join(request.headers);

  if (!requestHeaders.has("User-Agent"))
    requestHeaders.set("User-Agent", userAgent);
  setOpt(curl, CURLOPT_HTTPHEADER, requestHeaders.build());

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

  return std::shared_ptr<Response>(new Response {
    .url = request.url,
    .method = request.method,
    .body = std::move(body),
    .headers = std::move(responseHeaders),
    .statusCode = statusCode,
  });
}

long Client::download(const std::string &url, const std::string &path) const
{
  CURL *curl = init();
  if (curl == nullptr)
    return -1;

  FILE *fp = fopen(path.c_str(), "wb");
  if (fp == nullptr) {
    cleanup(curl);
    return -1;
  }

  setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  setOpt(curl, CURLOPT_URL, url.c_str());
  setOpt(curl, CURLOPT_WRITEDATA, fp);
  setOpt(curl, CURLOPT_WRITEFUNCTION, NULL);

  Headers headers {headers};
  if (!headers.has("User-Agent"))
    headers.set("User-Agent", userAgent);
  setOpt(curl, CURLOPT_HTTPHEADER, headers.build());

  if (rateLimiter != nullptr)
    rateLimiter->acquire();

  const auto code = perform(curl);
  long statusCode {};

  if (code == CURLE_OK)
    getInfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

  cleanup(curl);
  fclose(fp);

  if (statusCode != 200)
    fs::remove(path);

  return static_cast<int>(statusCode);
}

void Client::setCookies(Cookies *cookies)
{
  this->cookies = cookies;
}

void Client::setRateLimiter(RateLimiter *rateLimiter)
{
  this->rateLimiter = rateLimiter;
}

void Client::addInterceptor(std::shared_ptr<Interceptor> interceptor)
{
  if (interceptor == nullptr)
    return;
  interceptors.push_back(interceptor);
}

void Client::addInterceptor(Interceptor *interceptor)
{
  if (interceptor == nullptr)
    return;
  interceptors.push_back(std::shared_ptr<Interceptor>(interceptor));
}
}  // namespace Http