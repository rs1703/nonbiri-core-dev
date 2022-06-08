#ifndef NONBIRI_CORE_UTILITY_H_
#define NONBIRI_CORE_UTILITY_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define EXPORT_API extern "C" __declspec(dllexport)
#else
#  define EXPORT_API extern "C"
#endif
#include <map>
#include <string>
#include <vector>

namespace Utils
{
void *loadLibrary(const std::string &path);
void *getSymbol(void *handle, const std::string &symbol);
void freeLibrary(void *handle);

std::string stripPath(const std::string &url);
std::string stripDomain(const std::string &url);
std::string getFilename(const std::string &path);

bool strcmpi(const std::string &str1, const std::string &str2);

std::string capitalize(const std::string &str);
std::string lowercase(const std::string &str);
std::string uppercase(const std::string &str);
};  // namespace Utils

class SearchParams
{
  std::map<std::string, std::vector<std::string>> params;

public:
  explicit SearchParams() = default;
  explicit SearchParams(const std::vector<std::pair<std::string, std::string>> &params);
  explicit SearchParams(const std::string &url);

  bool has(const std::string &key) const;
  const std::string &get(const std::string &key) const;
  const std::vector<std::string> &getAll(const std::string &key) const;

  void add(const std::string &key, const std::string &value);
  void add(const std::string &key, const std::vector<std::string> &values);
  void set(const std::string &key, const std::string &value);
  void set(const std::string &key, const std::vector<std::string> &values);

  std::string toString() const;
};

#endif  // NONBIRI_CORE_UTILITY_H_