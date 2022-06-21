#include <core/object.h>

Object::Object(const std::string &k) : _key {k} {}

Json::Value StateOption::toJson() const
{
  Json::Value root {};
  if (!key.empty())
    root["key"] = key;
  if (!title.empty())
    root["title"] = title;
  if (!value.empty())
    root["value"] = value;
  if (state > State::OFF)
    root["state"] = state;
  if (defaultState > State::OFF)
    root["defaultState"] = defaultState;
  return root;
}

Json::Value StateObject::toJson() const
{
  Json::Value root {};
  if (!key.empty())
    root["key"] = key;
  if (!title.empty())
    root["title"] = title;
  if (!description.empty())
    root["description"] = description;
  if (!options.empty()) {
    for (const auto &option : options)
      root["options"].append(option.toJson());
  }
  return root;
}

Text::Text(const std::string &k, const std::string &v) : Object(key), key(k), value(v) {}
Json::Value Text::toJson() const
{
  Json::Value root {};
  root["value"] = value;
  return root;
}

Input::Input(const InputOption &o) : Object(InputOption::key), InputOption(o) {}
Json::Value Input::toJson() const
{
  Json::Value root {};
  if (!key.empty())
    root["key"] = key;
  if (!title.empty())
    root["title"] = title;
  if (!description.empty())
    root["description"] = description;
  if (!value.empty())
    root["value"] = value;
  if (!defaultValue.empty())
    root["defaultValue"] = defaultValue;
  root["type"] = type;
  return root;
}

Checkbox::Checkbox(const StateObject &o) : Object(StateObject::key), StateObject(o) {}
Json::Value Checkbox::toJson() const
{
  Json::Value root = StateObject::toJson();
  root["type"] = type;
  return root;
}

Radio::Radio(const StateObject &o) : Object(StateObject::key), StateObject(o) {}
Json::Value Radio::toJson() const
{
  Json::Value root = StateObject::toJson();
  root["type"] = type;
  return root;
}

TriState::TriState(const TriStateObject &o) : Object(TriStateObject::key), TriStateObject(o) {}
Json::Value TriState::toJson() const
{
  Json::Value root {};
  if (!key.empty())
    root["key"] = key;
  if (!excludedKey.empty())
    root["excludedKey"] = excludedKey;
  if (!title.empty())
    root["title"] = title;
  if (!description.empty())
    root["description"] = description;
  if (!options.empty()) {
    for (const auto &option : options)
      root["options"].append(option.toJson());
  }
  root["type"] = type;
  return root;
}

Select::Select(const StateObject &o) : Object(StateObject::key), StateObject(o) {}
Json::Value Select::toJson() const
{
  Json::Value root = StateObject::toJson();
  root["type"] = type;
  return root;
}
