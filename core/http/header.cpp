#include <algorithm>

#include <core/core.h>
#include <core/http/header.h>
#include <curl/curl.h>

namespace Http
{
Headers::Headers(const Headers &headers) : headers {headers.headers} {}
Headers::Headers(const std::map<std::string, std::vector<std::string>> &headers) : headers {headers} {}

Headers::~Headers()
{
  if (list != nullptr)
    slist_freeAll(list);
}

Headers &Headers::join(const Headers &headers)
{
  for (auto &header : headers.headers)
    this->headers[header.first] = header.second;
  return *this;
}

Headers &Headers::join(const std::map<std::string, std::vector<std::string>> &headers)
{
  for (auto &[key, values] : headers) {
    auto it = this->headers.find(key);
    if (it == this->headers.end()) {
      this->headers[key] = values;
    } else {
      for (auto &value : values) {
        if (std::find(it->second.begin(), it->second.end(), value) == it->second.end())
          this->headers[key].push_back(value);
      }
    }
  }
  return *this;
}

std::vector<std::string> Headers::get(const std::string &name) const
{
  auto it = headers.find(name);
  if (it == headers.end())
    return {};
  return it->second;
}

void Headers::add(const std::string &name, const std::string &value)
{
  auto it = headers.find(name);
  if (it == headers.end())
    headers[name] = {value};
  else {
    if (std::find(it->second.begin(), it->second.end(), value) == it->second.end())
      it->second.push_back(value);
  }
}

void Headers::set(const std::string &name, const std::string &value)
{
  headers[name] = {value};
}

void Headers::set(const std::string &name, const std::vector<std::string> &value)
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
  for (const auto &[key, values] : headers) {
    for (const auto &value : values)
      list = slist_append(list, std::string(key + ": " + value).c_str());
  }
  return list;
}

using iterator = std::map<std::string, std::vector<std::string>>::iterator;
using const_iterator = std::map<std::string, std::vector<std::string>>::const_iterator;

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
}  // namespace Http