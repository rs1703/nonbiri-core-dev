#include <filesystem>
#include <fstream>
#include <iostream>

#include <core/extension.h>
#include <core/pref.h>

namespace fs = std::filesystem;
fs::path prefsDir {"prefs"};

namespace Pref
{
Pref::Pref(const Info &info) : Info {info}
{
  for (size_t i = 0; i < info.options.size(); i++)
    index[info.options[i].first] = i;
}

Pref::Pref(const Json::Value &json)
{
  key         = json["key"].asString();
  title       = json["title"].asString();
  description = json["description"].asString();
  value       = json["value"];
}

Json::Value Pref::toJson(bool full) const
{
  Json::Value root {};
  root["key"]   = key;
  root["title"] = title;

  if (!description.empty())
    root["description"] = description;
  if (!value.empty())
    root["value"] = value;

  if (full) {
    for (const auto &[key, value] : options) {
      Json::Value option {};
      option["key"]   = key;
      option["value"] = value;
      root["options"].append(option);
    }
  }
  return root;
}

Checkbox::Checkbox(const Info &info) : Pref {info} {}
Checkbox::Checkbox(const Json::Value &json) : Pref {json} {}

Json::Value Checkbox::toJson(bool full) const
{
  Json::Value root = Pref::toJson(full);
  root["type"]     = "checkbox";
  return root;
}

ExcludableCheckbox::ExcludableCheckbox(const std::string &excludedKey, const Info &info) : Checkbox {info}, excludedKey {excludedKey} {}

ExcludableCheckbox::ExcludableCheckbox(const std::string &excludedKey, const Json::Value &json) : Checkbox {json}, excludedKey {excludedKey}
{
}

Json::Value ExcludableCheckbox::toJson(bool full) const
{
  Json::Value root    = Checkbox::toJson(full);
  root["excludedKey"] = excludedKey;
  return root;
}

Radio::Radio(const Info &info) : Pref {info} {}
Radio::Radio(const Json::Value &json) : Pref {json} {}

Json::Value Radio::toJson(bool full) const
{
  Json::Value root = Pref::toJson(full);
  root["type"]     = "radio";
  return root;
}

Select::Select(const Info &info) : Pref {info} {}
Select::Select(const Json::Value &json) : Pref {json} {}

Json::Value Select::toJson(bool full) const
{
  Json::Value root = Pref::toJson(full);
  root["type"]     = "select";
  return root;
}

Prefs::Prefs(const std::string &id)
{
  std::cout << "Prefs::Prefs" << std::endl;
  path = prefsDir / (id + ".json");
  if (!fs::exists(prefsDir))
    fs::create_directory(prefsDir);

  if (!fs::exists(path))
    return;

  std::ifstream file {path};
  if (!file.is_open())
    return;

  Json::Value root {};
  file >> root;
  file.close();

  for (const auto &pref : root) {
    const std::string type = pref["type"].asString();
    if (type == "checkbox") {
      const std::string excludedKey = pref["excludedKey"].asString();
      if (excludedKey.empty())
        add(new Checkbox {pref});
      else
        add(new ExcludableCheckbox {excludedKey, pref});
    } else if (type == "radio")
      add(new Radio {pref});
    else if (type == "select")
      add(new Select {pref});
  }
}

Prefs::~Prefs()
{
  if (!prefs.empty())
    save();
}

void Prefs::add(Pref *pref)
{
  if (pref == nullptr)
    return;

  auto it = index.find(pref->key);
  if (it == index.end()) {
    prefs.push_back(std::shared_ptr<Pref> {pref});
    index[pref->key] = prefs.size() - 1;
  } else {
    pref->value = prefs[it->second]->value;
    delete prefs[it->second].get();
    prefs[it->second] = std::shared_ptr<Pref> {pref};
  }
  save();
}

const std::shared_ptr<Pref> Prefs::get(const std::string &key) const
{
  auto it = index.find(key);
  if (it == index.end())
    return nullptr;
  return prefs[it->second];
}

const std::vector<std::shared_ptr<Pref>> &Prefs::get() const
{
  return prefs;
}

const std::map<std::string, size_t> &Prefs::getIndex() const
{
  return index;
}

int Prefs::set(const std::string &key, const Json::Value &value)
{
  auto it = index.find(key);
  if (it == index.end())
    return -1;

  prefs[it->second]->value = value;
  save();
  return 0;
}

void Prefs::save() const
{
  std::ofstream file {path};
  if (!file.is_open())
    return;

  Json::StyledWriter writer;
  file << writer.write(toJson());
  file.close();
}

Json::Value Prefs::toJson(bool full) const
{
  Json::Value root {};
  for (const auto &pref : prefs)
    root.append(pref->toJson(full));
  return root;
}
}  // namespace Pref