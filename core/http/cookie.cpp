#include <sstream>

#include <core/core.h>
#include <core/http/cookie.h>
#include <core/utility.h>
#include <curl/curl.h>
#include <string.h>

namespace Http
{

std::string Cookie::toString() const
{
  std::stringstream ss;
  ss << name << "=" << value;
  if (!path.empty())
    ss << "; path=" << path;
  if (!domain.empty())
    ss << "; domain=" << domain;
  if (maxAge > 0)
    ss << "; max-age=" << maxAge;
  if (httpOnly)
    ss << "; httponly";
  if (secure)
    ss << "; secure";
  return ss.str();
}

Cookie Cookie::parse(const std::string &str)
{
  Cookie cookie {};

  auto start = 0U;
  auto end = str.find(';');

  while (end != std::string::npos) {
    auto field = str.substr(start, end - start);
    auto key = Utils::lowercase(field.substr(0, field.find('=')));
    auto value = field.substr(field.find('=') + 1);

    if (key == "name")
      cookie.name = value;
    else if (key == "value")
      cookie.value = value;
    else if (key == "path")
      cookie.path = value;
    else if (key == "domain")
      cookie.domain = value;
    else if (key == "httponly")
      cookie.httpOnly = true;
    else if (key == "secure")
      cookie.secure = true;
    else if (key == "max-age")
      cookie.maxAge = std::stoll(value);

    start = end + 1;
    end = str.find(';', start);
  }
  return cookie;
}

Cookies::Cookies(const std::string &domain) : domain(domain) {}
Cookies::~Cookies() {}

void Cookies::add(const Cookie &cookie)
{
  Cookie c {cookie};
  if (c.domain.empty())
    c.domain = domain;
  if (c.path.empty())
    c.path = "/";

  auto it = cookies.find(c.domain);
  if (it == cookies.end()) {
    cookies[c.domain] = std::map<std::string, Cookie>({{c.path, c}});
  } else {
    it->second[c.name] = c;
  }
}

void Cookies::remove(const std::string &name)
{
  remove(domain, name);
}

void Cookies::remove(const std::string &domain, const std::string &name)
{
  auto it = cookies.find(domain);
  if (it == cookies.end())
    return;
  it->second.erase(name);
}

bool Cookies::has(const std::string &name) const
{
  return has(domain, name);
}

bool Cookies::has(const std::string &domain, const std::string &name) const
{
  auto it = cookies.find(domain);
  if (it == cookies.end())
    return false;
  return it->second.find(name) != it->second.end();
}

bool Cookies::empty() const
{
  return cookies.empty();
}

void Cookies::clear()
{
  cookies.clear();
}

void Cookies::build(const std::string &url, void *curl)
{
  for (const auto &[domain, cookies] : cookies) {
    if (url.find(domain) != std::string::npos) {
      std::string cookieString {};
      for (const auto &[_, cookie] : cookies)
        cookieString += cookie.name + "=" + cookie.value + "; ";
      setOpt(curl, CURLOPT_COOKIE, cookieString.c_str());
    }
  }
}

using iterator = std::map<std::string, std::map<std::string, Cookie>>::iterator;
using const_iterator = std::map<std::string, std::map<std::string, Cookie>>::const_iterator;

iterator Cookies::begin()
{
  return cookies.begin();
}

iterator Cookies::end()
{
  return cookies.end();
}

const_iterator Cookies::begin() const
{
  return cookies.begin();
}

const_iterator Cookies::end() const
{
  return cookies.end();
}
}  // namespace Http