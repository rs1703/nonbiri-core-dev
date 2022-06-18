#ifndef NONBIRI_CORE_HTTP_HEADER_H_
#define NONBIRI_CORE_HTTP_HEADER_H_

#include <map>
#include <string>

struct curl_slist;

namespace Http
{
class Headers
{
  friend class Client;

  std::map<std::string, std::string> headers;
  struct curl_slist *list {nullptr};

public:
  Headers() = default;
  Headers(const Headers &headers);
  Headers(const std::map<std::string, std::string> &headers);
  ~Headers();

  void join(const Headers &headers);
  void join(const std::map<std::string, std::string> &headers);

  void set(const std::string &name, const std::string &value);
  void remove(const std::string &name);
  bool has(const std::string &name) const;
  bool empty() const;
  void clear();

  using iterator = std::map<std::string, std::string>::iterator;
  using const_iterator = std::map<std::string, std::string>::const_iterator;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

private:
  struct curl_slist *build();
};
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_HEADER_H_