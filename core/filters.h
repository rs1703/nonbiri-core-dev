#ifndef NONBIRI_CORE_FILTERS_H_
#define NONBIRI_CORE_FILTERS_H_

#include <string>
#include <vector>

#include <json/json.h>

struct Filter
{
  friend class Manager;
  friend class Api;
  friend class Web;

  using string = std::string;

  struct Pair
  {
    friend class Filter;

    const string key {};
    const string value {};

  private:
    Json::Value toJson() const;
  };

  struct Option
  {
    friend class Filter;

    const string key {};
    const string value {};
    const bool defaultOption {};

  private:
    Json::Value toJson() const;
  };

  using Options = std::vector<Option>;

  const string key {};
  const string title {};
  const string description {};
  const Options options {};

  Filter() = default;
  Filter(const Filter &);

  Filter(const string &key, const string &title, const Options &options, const string &type);
  Filter(const string &key, const string &title, const string &description, const Options &options, const string &type);

private:
  string type {};

  Json::Value toJson() const;
  string toString() const;
};

class Filters
{
  std::map<std::string, const Filter> filters {};

public:
  Filters() = default;

  void add(const Filter &filter);
  void remove(const Filter &filter);
  void remove(const std::string &key);
  const std::map<std::string, const Filter> &get() const;
};

struct Checkbox : public Filter
{
  using string  = std::string;
  using Options = std::vector<Filter::Option>;

  Checkbox(const string &key, const string &title, const Options &options);
  Checkbox(const string &key, const string &title, const string &description, const Options &options);
};

struct Radio : public Filter
{
  using string  = std::string;
  using Options = std::vector<Filter::Option>;

  Radio(const string &key, const string &title, const Options &options);
  Radio(const string &key, const string &title, const string &description, const Options &options);
};

struct Select : public Filter
{
  using string  = std::string;
  using Options = std::vector<Filter::Option>;

  Select(const string &key, const string &title, const Options &options);
  Select(const string &key, const string &title, const string &description, const Options &options);
};

#endif  // NONBIRI_CORE_FILTERS_H_