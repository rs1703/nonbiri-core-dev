#ifndef NONBIRI_CORE_UTILS_H_
#define NONBIRI_CORE_UTILS_H_

#include <string>

namespace http
{
std::string get(const std::string &url);
}  // namespace http

std::string stripPath(const std::string &url);
std::string stripDomain(const std::string &url);
std::string getFilename(const std::string &path);

#endif  // NONBIRI_CORE_UTILS_H_