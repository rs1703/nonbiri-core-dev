#include <core/utils/utils.h>
#include <curl/curl.h>

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string http::get(const std::string &url)
{
  CURL *curl = curl_easy_init();
  if (!curl)
    return NULL;

  std::string readBuffer;

  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return readBuffer;
}

std::string stripPath(const std::string &url)
{
  if (url.find("://") == std::string::npos)
    return url;

  return url.substr(0, url.find("/", 8));
}

std::string stripDomain(const std::string &url)
{
  if (url.find("://") == std::string::npos)
    return url;

  size_t pos = url.find("/", 8);
  if (pos == std::string::npos)
    return "/";
  return url.substr(pos);
}

std::string getFilename(const std::string &path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define SEPARATOR "\\"
#else
#  define SEPARATOR "/"
#endif
  auto pos = path.find_last_of(SEPARATOR);
  if (pos == std::string::npos)
    return path;
  return path.substr(pos + 1);
}