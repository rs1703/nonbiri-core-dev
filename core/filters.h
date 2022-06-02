#ifndef NONBIRI_CORE_FILTERS_H_
#define NONBIRI_CORE_FILTERS_H_

#include <map>
#include <string>

#include <json/json.h>

struct FilterKV
{
  std::string key {};
  std::string value {};
};

struct Filter
{
  std::string name {};
  std::string key {};
  std::map<std::string, std::string> options {};

  Filter() = default;
  Filter(const std::string &name,
         const std::string &key,
         const std::map<std::string, std::string> &options,
         const std::string &type = "")
    : name {name}, key {key}, options {options}, type {type}
  {
  }

  Json::Value toJson() const;
  std::string toString() const;

private:
  std::string type {};
};

struct Checkbox : public Filter
{
  Checkbox(const std::string &name, const std::string &key, const std::map<std::string, std::string> &options)
    : Filter {name, key, options, "checkbox"}
  {
  }
};

struct Radio : public Filter
{
  Radio(const std::string &name, const std::string &key, const std::map<std::string, std::string> &options)
    : Filter {name, key, options, "radio"}
  {
  }
};

struct Select : public Filter
{
  Select(const std::string &name, const std::string &key, const std::map<std::string, std::string> &options)
    : Filter {name, key, options, "select"}
  {
  }
};

#endif  // NONBIRI_CORE_FILTERS_H_