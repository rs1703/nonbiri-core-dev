#ifndef NONBIRI_CORE_FILTER_H_
#define NONBIRI_CORE_FILTER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <json/json.h>

class Extension;
class Manager;
class Api;
class Web;

namespace Filter
{
enum State
{
  DISABLED,
  ENABLED,
  EXCLUDED,
};

struct Option
{
  friend class Filter;

  const std::string key {};
  const std::string value {};
  const State defaultState {DISABLED};

private:
  Json::Value toJson() const;
};

struct Info
{
  const std::string key {};
  const std::string title {};
  const std::string description {};
  const std::vector<Option> options {};
  const bool hidden {};
};

struct Filter : public Info
{
  friend class Filters;
  friend class ::Manager;
  friend class ::Api;
  friend class ::Web;

  Filter() = delete;
  Filter(const Info &info);
  virtual ~Filter() = default;

protected:
  virtual Json::Value toJson() const;
};

struct Hidden : public Filter
{
  Hidden(const std::string &key);
};

struct Checkbox : public Filter
{
  Checkbox(const Info &info);

protected:
  virtual Json::Value toJson() const override;
};

struct ExcludableCheckbox : public Checkbox
{
  const std::string excludedKey {};
  ExcludableCheckbox(const std::string &excludedKey, const Info &info);

protected:
  virtual Json::Value toJson() const override;
};

struct Radio : public Filter
{
  Radio(const Info &info);

protected:
  virtual Json::Value toJson() const override;
};

struct Select : public Filter
{
  Select(const Info &info);

protected:
  virtual Json::Value toJson() const override;
};

class Filters
{
  friend class ::Extension;
  friend class ::Manager;
  friend class ::Api;
  friend class ::Web;

  std::vector<std::shared_ptr<const Filter>> filters {};
  std::map<std::string, size_t> index {};

public:
  Filters() = default;

  void add(const Filter *filter);

private:
  const std::vector<std::shared_ptr<const Filter>> &get() const;
  const std::map<std::string, size_t> &getIndex() const;
};
}  // namespace Filter

#endif  // NONBIRI_CORE_FILTER_H_