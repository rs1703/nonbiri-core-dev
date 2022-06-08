#ifndef NONBIRI_CORE_FILTERS_H_
#define NONBIRI_CORE_FILTERS_H_

#include <memory>
#include <string>
#include <unordered_map>
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
  const bool isHidden {};

  Filter() = default;
  Filter(const Filter &);

  Filter(const std::string &key);
  Filter(const string &key, const string &title, const Options &options, const string &type);
  Filter(const string &key, const string &title, const string &description, const Options &options, const string &type);

protected:
  virtual Json::Value toJson() const;

private:
  string type {};
  string toString() const;
};

class Filters
{
  std::unordered_map<std::string, std::shared_ptr<const Filter>> filters {};

public:
  Filters() = default;

  void add(const Filter *filter);
  void remove(const Filter *filter);
  void remove(const std::string &key);
  const std::unordered_map<std::string, std::shared_ptr<const Filter>> &get() const;
};

struct Hidden : public Filter
{
  Hidden(const std::string &key);
};

struct Checkbox : public Filter
{
  using string  = std::string;
  using Options = std::vector<Filter::Option>;

  Checkbox(const string &key, const string &title, const Options &options);
  Checkbox(const string &key, const string &title, const string &description, const Options &options);
};

struct ExcludableCheckbox : public Filter
{
  using string  = std::string;
  using Options = std::vector<Filter::Option>;

  const string excludedKey {};

  ExcludableCheckbox(const string &key, const string &excludedKey, const string &title, const Options &options);
  ExcludableCheckbox(const string &key,
    const string &excludedKey,
    const string &title,
    const string &description,
    const Options &options);

  Json::Value toJson() const override;
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