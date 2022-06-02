#include <stdexcept>
#include <thread>

#include <core/http.h>
#include <core/utility.h>

Http::init_t Http::init {nullptr};
Http::setOpt_t Http::setOpt {nullptr};
Http::perform_t Http::perform {nullptr};
Http::cleanup_t Http::cleanup {nullptr};
Http::getInfo_t Http::getInfo {nullptr};

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

void Http::initialize(
  init_t initPtr, setOpt_t setOptPtr, perform_t performPtr, cleanup_t cleanupPtr, getInfo_t getInfoPtr)
{
  init = initPtr;
  setOpt = setOptPtr;
  perform = performPtr;
  cleanup = cleanupPtr;
  getInfo = getInfoPtr;
}

std::string Http::get(const std::string &url)
{
  CURL *curl = Http::init();
  if (curl == nullptr)
    return NULL;

  std::string readBuffer {};

  Http::setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  Http::setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  Http::setOpt(curl, CURLOPT_URL, url.c_str());
  Http::setOpt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  Http::setOpt(curl, CURLOPT_WRITEDATA, &readBuffer);

  Http::perform(curl);
  Http::cleanup(curl);

  return readBuffer;
}

int Http::download(const std::string &url, const std::string &path)
{
  CURL *curl = Http::init();
  if (curl == nullptr)
    return -1;

  Http::setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  Http::setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  FILE *fp = fopen(path.c_str(), "wb");
  if (fp == nullptr) {
    Http::cleanup(curl);
    return -1;
  }

  Http::setOpt(curl, CURLOPT_URL, url.c_str());
  Http::setOpt(curl, CURLOPT_WRITEDATA, fp);
  Http::setOpt(curl, CURLOPT_WRITEFUNCTION, NULL);

  const CURLcode code = Http::perform(curl);
  long httpCode = -1;

  if (code == CURLE_OK)
    Http::getInfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

  Http::cleanup(curl);
  fclose(fp);

  return httpCode;
}

RateLimiter::RateLimiter(int64_t size, std::chrono::seconds interval) : tokens {size}, size {size}, interval {interval}
{
}

void RateLimiter::acquire()
{
  MutexLock lock {mutex};
  auto now = std::chrono::system_clock::now();
  if (lastTime + interval < now) {
    lastTime = now;
    tokens = size;
  } else if (tokens <= 0) {
    std::this_thread::sleep_until(lastTime + interval);
    tokens = size;
  }
  --tokens;
}

HttpClient::HttpClient(RateLimiter *rateLimiter) : rateLimiter {rateLimiter} {}

HttpClient::~HttpClient()
{
  if (rateLimiter != nullptr)
    delete rateLimiter;
}

std::string HttpClient::get(const std::string &url) const
{
  CURL *curl = Http::init();
  if (curl == nullptr)
    return NULL;

  std::string readBuffer {};
  Http::setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  Http::setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  Http::setOpt(curl, CURLOPT_URL, url.c_str());
  Http::setOpt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  Http::setOpt(curl, CURLOPT_WRITEDATA, &readBuffer);

  if (rateLimiter != nullptr)
    rateLimiter->acquire();

  Http::perform(curl);
  Http::cleanup(curl);

  return readBuffer;
}

int HttpClient::download(const std::string &url, const std::string &path) const
{
  CURL *curl = Http::init();
  if (curl == nullptr)
    return -1;

  Http::setOpt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  Http::setOpt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  FILE *fp = fopen(path.c_str(), "wb");
  if (fp == nullptr) {
    Http::cleanup(curl);
    return -1;
  }

  Http::setOpt(curl, CURLOPT_URL, url.c_str());
  Http::setOpt(curl, CURLOPT_WRITEDATA, fp);
  Http::setOpt(curl, CURLOPT_WRITEFUNCTION, NULL);

  if (rateLimiter != nullptr)
    rateLimiter->acquire();

  const CURLcode code = Http::perform(curl);
  long httpCode = -1;

  if (code == CURLE_OK)
    Http::getInfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

  Http::cleanup(curl);
  fclose(fp);

  return httpCode;
}
