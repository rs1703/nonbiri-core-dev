#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif
#include <core/utility.h>

namespace Utils
{
void *loadLibrary(const std::string &path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return LoadLibrary(path.c_str());
#else
  return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void *getSymbol(void *handle, const std::string &symbol)
{
  if (handle == nullptr)
    return nullptr;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return GetProcAddress((HMODULE)handle, symbol.c_str());
#else
  return dlsym(handle, symbol.c_str());
#endif
}

void freeLibrary(void *handle)
{
  if (handle == nullptr)
    return;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  FreeLibrary((HMODULE)handle);
#else
  dlclose(handle);
#endif
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

bool strcmpi(const std::string &str1, const std::string &str2)
{
  if (str1.size() != str2.size())
    return false;
  for (size_t i = 0; i < str1.size(); i++) {
    if (tolower(str1[i]) != tolower(str2[i]))
      return false;
  }
  return true;
}

std::string capitalize(const std::string &str)
{
  std::string result = str;
  for (size_t i {0}; i < result.size(); i++) {
    if (std::isalpha(result[i]) && (i == 0 || std::isspace(result[i - 1])))
      result[i] = toupper(result[i]);
  }
  return result;
}

std::string lowercase(const std::string &str)
{
  std::string result = str;
  for (size_t i = 0; i < result.size(); i++)
    result[i] = tolower(result[i]);
  return result;
}

std::string uppercase(const std::string &str)
{
  std::string result = str;
  for (size_t i = 0; i < result.size(); i++)
    result[i] = toupper(result[i]);
  return result;
}
}  // namespace Utils

SearchParams::SearchParams(const std::vector<std::pair<std::string, std::string>> &params)
{
  for (auto &param : params)
    add(param.first, param.second);
}

SearchParams::SearchParams(const std::string &url)
{
  size_t pos {0};
  size_t length {url.length()};

  while (pos < length) {
    size_t next {url.find('&', pos)};
    if (next == std::string::npos)
      next = length;

    size_t eq {url.find('=', pos)};
    if (eq == std::string::npos || eq > next)
      break;

    std::string key {url.substr(pos, eq - pos)};
    std::string val {url.substr(eq + 1, next - eq - 1)};
    params[key].push_back(val);
    pos = next + 1;
  }
}

bool SearchParams::has(const std::string &key) const
{
  return params.find(key) != params.end();
}

std::string SearchParams::get(const std::string &key) const
{
  auto it {params.find(key)};
  if (it == params.end())
    return NULL;
  return it->second.front();
}

std::vector<std::string> SearchParams::getAll(const std::string &key) const
{
  auto it {params.find(key)};
  if (it == params.end())
    return {};
  return it->second;
}

void SearchParams::add(const std::string &key, const std::string &value)
{
  if (params.find(key) == params.end())
    params[key] = {value};
  else
    params[key].push_back(value);
}

void SearchParams::add(const std::string &key, const std::vector<std::string> &values)
{
  if (params.find(key) == params.end())
    params[key] = values;
  else
    params[key].insert(params[key].end(), values.begin(), values.end());
}

void SearchParams::set(const std::string &key, const std::string &value)
{
  params[key] = {value};
}

void SearchParams::set(const std::string &key, const std::vector<std::string> &values)
{
  params[key] = values;
}

std::string SearchParams::toString() const
{
  std::string result {};
  size_t i {0};

  for (const auto &[key, values] : params) {
    for (const auto &value : values) {
      if (i > 0)
        result += '&';
      result += key + '=' + value;
      i++;
    }
  }

  return result;
}
