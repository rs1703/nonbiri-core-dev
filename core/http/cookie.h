#ifndef NONBIRI_CORE_HTTP_COOKIE_H_
#define NONBIRI_CORE_HTTP_COOKIE_H_

#include <map>
#include <memory>
#include <string>

namespace Http
{
struct Cookie
{
  friend class Cookies;

  std::string hostname {};
  bool includeSubdomains {};
  std::string path {};
  bool secure {};
  int64_t maxAge {};
  std::string name {};
  std::string value {};

private:
  char *c_str() const;
};

class Cookies
{
  friend class Client;

  std::map<std::string, Cookie> cookies;
  const std::string hostname;

public:
  Cookies(const std::string &hostname);
  ~Cookies();

  void add(const Cookie &cookie);
  void remove(const std::string &name);
  bool has(const std::string &name) const;
  bool empty() const;
  void clear();

  using iterator = std::map<std::string, Cookie>::iterator;
  using const_iterator = std::map<std::string, Cookie>::const_iterator;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

private:
  void build(void *curl);
};
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_COOKIE_H_