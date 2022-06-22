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

  std::string name {};
  std::string value {};

  std::string path {};
  std::string domain {};

  bool httpOnly {};
  bool secure {};
  int64_t maxAge {};

  static Cookie parse(const std::string &str);

private:
  std::string toString() const;
};

class Cookies
{
  friend class Client;

  std::map<std::string, std::map<std::string, Cookie>> cookies;
  const std::string domain;

public:
  Cookies(const std::string &domain);
  ~Cookies();

  void add(const Cookie &cookie);
  void remove(const std::string &name);
  void remove(const std::string &domain, const std::string &name);
  bool has(const std::string &name) const;
  bool has(const std::string &domain, const std::string &name) const;
  bool empty() const;
  void clear();

  using iterator = std::map<std::string, std::map<std::string, Cookie>>::iterator;
  using const_iterator = std::map<std::string, std::map<std::string, Cookie>>::const_iterator;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

private:
  void build(const std::string &url, void *curl);
};
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_COOKIE_H_