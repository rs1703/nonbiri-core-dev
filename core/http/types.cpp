#include <core/core.h>
#include <core/http/types.h>
#include <curl/curl.h>

namespace Http
{
Headers::Headers(const Headers &headers) : headers {headers.headers} {}
Headers::Headers(const std::map<std::string, std::string> &headers) : headers {headers} {}

Headers::~Headers()
{
  if (list != nullptr)
    slist_freeAll(list);
}

void Headers::join(const Headers &headers)
{
  for (auto &header : headers.headers)
    this->headers[header.first] = header.second;
}

void Headers::join(const std::map<std::string, std::string> &headers)
{
  for (auto &header : headers)
    this->headers[header.first] = header.second;
}

void Headers::set(const std::string &name, const std::string &value)
{
  headers[name] = value;
}

void Headers::remove(const std::string &name)
{
  headers.erase(name);
}

bool Headers::has(const std::string &name) const
{
  return headers.find(name) != headers.end();
}

bool Headers::empty() const
{
  return headers.empty();
}

void Headers::clear()
{
  headers.clear();
}

curl_slist *Headers::build()
{
  for (auto &header : headers)
    list = slist_append(list, std::string(header.first + ": " + header.second).c_str());
  return list;
}

using iterator = std::map<std::string, std::string>::iterator;
using const_iterator = std::map<std::string, std::string>::const_iterator;

iterator Headers::begin()
{
  return headers.begin();
}

iterator Headers::end()
{
  return headers.end();
}

const_iterator Headers::begin() const
{
  return headers.begin();
}

const_iterator Headers::end() const
{
  return headers.end();
}
};  // namespace Http