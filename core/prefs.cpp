#include <filesystem>
#include <fstream>
#include <iostream>

#include <core/extension.h>
#include <core/prefs.h>

namespace fs = std::filesystem;

fs::path prefsDir {"prefs"};

Json::Value Pref::toJson() const
{
  Json::Value root {};
  root["key"]         = key;
  root["title"]       = title;
  root["description"] = description;

  if (!value.isNull())
    root["value"] = value;
  for (const auto &value : values)
    root["values"].append(value);
  for (const auto &option : options)
    root["options"][option.first] = option.second;

  return root;
}

Prefs::Prefs(const Extension &extension, const std::vector<Pref> &defaults) :
  extension {extension},
  path {prefsDir / (extension.id + ".json")}
{
  for (auto &pref : defaults)
    prefs.emplace(pref.key, pref);

  if (!fs::exists(prefsDir))
    fs::create_directory(prefsDir);

  if (!fs::exists(path)) {
    save();
    return;
  }

  std::ifstream file {path};
  if (!file.is_open())
    return;

  Json::Value root {};
  file >> root;
  file.close();

  for (const auto &pref : root) {
    const auto key         = pref["key"].asString();
    const auto title       = pref["title"].asString();
    const auto description = pref["description"].asString();

    std::vector<Json::Value> values {};
    for (const auto &v : pref["values"])
      values.push_back(v);
    std::map<std::string, Json::Value> options {};
    for (const auto &k : pref["options"].getMemberNames())
      options[k] = pref["options"][k];

    prefs.emplace(key,
      Pref {
        .key         = key,
        .title       = title,
        .description = description,
        .value       = pref["value"],
        .values      = values,
        .options     = options,
      });
  }
}

Prefs::~Prefs()
{
  save();
}

Prefs *Prefs::getInstance() const
{
  return const_cast<Prefs *>(this);
}

const Pref *Prefs::getPref(const std::string &key) const
{
  if (prefs.find(key) == prefs.end())
    return nullptr;
  return &prefs.at(key);
}

const Pref *Prefs::getPref(const std::string &key, const Pref &defaultPref)
{
  if (prefs.find(key) == prefs.end())
    prefs.emplace(key, defaultPref);
  return &prefs.at(key);
}

int Prefs::setPref(const std::string &key, const Json::Value &value)
{
  if (prefs.find(key) == prefs.end())
    return 1;

  auto &pref = prefs.at(key);
  pref.value = value;

  save();
  return 0;
}

int Prefs::setPref(const std::string &key, const std::string &valueKey)
{
  if (prefs.find(key) == prefs.end())
    return 1;

  auto &pref = prefs[key];
  if (pref.options.find(valueKey) == pref.options.end())
    return 2;

  pref.value = pref.options.at(valueKey);

  save();
  return 0;
}

int Prefs::setPref(const std::string &key, const std::vector<Json::Value> &value)
{
  if (prefs.find(key) == prefs.end())
    return 1;

  auto &pref  = prefs.at(key);
  pref.values = value;

  save();
  return 0;
}

int Prefs::setPref(const std::string &key, const std::vector<std::string> &valueKeys)
{
  if (prefs.find(key) == prefs.end())
    return 1;

  auto &pref = prefs[key];
  pref.values.clear();
  for (const auto &valueKey : valueKeys) {
    if (pref.options.find(valueKey) == pref.options.end())
      return 2;
    pref.values.push_back(pref.options.at(valueKey));
  }

  save();
  return 0;
}

void Prefs::save() const
{
  std::ofstream file {path};
  if (!file.is_open())
    return;

  Json::Value root {};
  for (const auto &pref : prefs)
    root.append(pref.second.toJson());

  Json::StyledWriter writer;
  file << writer.write(root);
  file.close();
}