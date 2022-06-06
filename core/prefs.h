#ifndef NONBIRI_CORE_PREFS_H_
#define NONBIRI_CORE_PREFS_H_

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <json/json.h>

class Extension;

struct Pref
{
  friend class Prefs;

  const std::string key {};
  const std::string title {};
  const std::string description {};

  Json::Value value {};
  std::vector<Json::Value> values {};
  const std::map<std::string, Json::Value> options {};

private:
  Json::Value toJson() const;
};

class Prefs
{
  const Extension &extension;
  std::map<std::string, Pref> prefs {};
  std::filesystem::path path {};

public:
  Prefs(const Extension &extension, const std::vector<Pref> &defaults = {});
  ~Prefs();

  Prefs *getInstance() const;
  const Pref *getPref(const std::string &key) const;
  const Pref *getPref(const std::string &key, const Pref &defaultPref);

  int setPref(const std::string &key, const Json::Value &value);
  int setPref(const std::string &key, const std::string &valueKey);
  int setPref(const std::string &key, const std::vector<Json::Value> &values);
  int setPref(const std::string &key, const std::vector<std::string> &valueKeys);

private:
  void save() const;
};

#endif  // NONBIRI_CORE_PREFS_H_