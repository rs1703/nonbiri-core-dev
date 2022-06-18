#ifndef NONBIRI_CORE_PREF_H_
#define NONBIRI_CORE_PREF_H_

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <json/json.h>

class Extension;
class Manager;
class Api;
class Web;

namespace Pref
{
struct Info
{
  std::string key {};
  std::string title {};
  std::string description {};
  Json::Value value {};
  std::vector<std::pair<std::string, Json::Value>> options {};
};

struct Pref;
using OnChangeFn = std::function<void(const Pref &)>;

struct Pref : public Info
{
  friend class Prefs;
  friend class ::Manager;
  friend class ::Api;
  friend class ::Web;

  Pref() = delete;
  Pref(const Info &info, OnChangeFn onChange = nullptr);
  Pref(const Json::Value &json, OnChangeFn onChange = nullptr);
  virtual ~Pref() = default;

  void onChange(OnChangeFn onChange);

private:
  std::map<std::string, size_t> index {};
  OnChangeFn m_onChange {nullptr};

protected:
  virtual Json::Value toJson(bool full = false) const;
};

struct Input : public Pref
{
  Input(const Info &info, OnChangeFn onChange = nullptr);
  Input(const Json::Value &json, OnChangeFn onChange = nullptr);

protected:
  virtual Json::Value toJson(bool full = false) const override;
};

struct Checkbox : public Pref
{
  Checkbox(const Info &info, OnChangeFn onChange = nullptr);
  Checkbox(const Json::Value &json, OnChangeFn onChange = nullptr);

protected:
  virtual Json::Value toJson(bool full = false) const override;
};

struct ExcludableCheckbox : public Checkbox
{
  const std::string excludedKey {};
  ExcludableCheckbox(const std::string &excludedKey, const Info &info, OnChangeFn onChange = nullptr);
  ExcludableCheckbox(const std::string &excludedKey, const Json::Value &json, OnChangeFn onChange = nullptr);

protected:
  virtual Json::Value toJson(bool full = false) const override;
};

struct Radio : public Pref
{
  Radio(const Info &info, OnChangeFn onChange = nullptr);
  Radio(const Json::Value &json, OnChangeFn onChange = nullptr);

protected:
  virtual Json::Value toJson(bool full = false) const override;
};

struct Select : public Pref
{
  Select(const Info &info, OnChangeFn onChange = nullptr);
  Select(const Json::Value &json, OnChangeFn onChange = nullptr);

protected:
  virtual Json::Value toJson(bool full = false) const override;
};

class Prefs
{
  friend class ::Extension;
  friend class ::Manager;
  friend class ::Api;
  friend class ::Web;

  std::vector<std::shared_ptr<Pref>> prefs {};
  std::map<std::string, size_t> index {};
  std::filesystem::path path {};

public:
  Prefs() = delete;
  Prefs(const std::string &id);
  virtual ~Prefs();

  void add(Pref *pref);
  const std::shared_ptr<Pref> get(const std::string &key) const;

private:
  const std::vector<std::shared_ptr<Pref>> &get() const;
  const std::map<std::string, size_t> &getIndex() const;

  int set(const std::string &key, const Json::Value &value);
  void save() const;

  Json::Value toJson(bool full = false) const;
};
}  // namespace Pref

#endif  // NONBIRI_CORE_PREF_H_