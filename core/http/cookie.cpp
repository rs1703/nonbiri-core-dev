#include <iostream>
#include <sstream>

#include <core/core.h>
#include <core/http/cookie.h>
#include <curl/curl.h>
#include <string.h>

namespace Http
{
Cookies::Cookies(const std::string &hostname) : hostname(hostname) {}
Cookies::~Cookies() {}

void Cookies::add(const Cookie &cookie)
{
  cookies[cookie.name] = cookie;
  if (cookie.hostname.empty())
    cookies[cookie.name].hostname = hostname;
  if (cookie.path.empty())
    cookies[cookie.name].path = "/";
}

void Cookies::remove(const std::string &name)
{
  cookies.erase(name);
}

bool Cookies::has(const std::string &name) const
{
  return cookies.find(name) != cookies.end();
}

bool Cookies::empty() const
{
  return cookies.empty();
}

void Cookies::clear()
{
  cookies.clear();
}

void Cookies::build(void *curl)
{
  for (const auto &[_, cookie] : cookies)
    setOpt(curl, CURLOPT_COOKIE, (cookie.name + "=" + cookie.value + ";").c_str());
}

using iterator = std::map<std::string, Cookie>::iterator;
using const_iterator = std::map<std::string, Cookie>::const_iterator;

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