#ifndef NONBIRI_CORE_OBJECT_H_
#define NONBIRI_CORE_OBJECT_H_

#include <memory>
#include <string>
#include <vector>

#include <json/json.h>

namespace State
{
const int OFF = 0;
const int ON = 1;
const int EX = 2;
}  // namespace State

struct InputOption
{
  const std::string key {};
  std::string title {};
  std::string description {};
  std::string value {};
  const std::string defaultValue {};
};

struct StateOption
{
  const std::string key {};
  std::string title {};
  std::string value;
  int state {};
  const int defaultState {};

  Json::Value toJson() const;
};

struct Object
{
  friend class Group;
  friend class Filters;
  friend class Prefs;

  const std::string &_key {};
  Object(const std::string &key);
  virtual Json::Value toJson() const = 0;
};

struct StateObject
{
  const std::string key {};
  std::string title {};
  std::string description {};
  std::vector<StateOption> options {};

  Json::Value toJson() const;
};

struct TriStateObject
{
  const std::string key {};
  std::string excludedKey {};
  std::string title {};
  std::string description {};
  std::vector<StateOption> options {};
};

struct Text : public Object
{
  inline static std::string type {"text"};
  const std::string key {};
  std::string value {};

  explicit Text(const std::string &key, const std::string &value);
  Json::Value toJson() const override;
};

struct Input : public Object, public InputOption
{
  inline static std::string type {"input"};
  explicit Input(const InputOption &);
  Json::Value toJson() const override;
};

struct Checkbox : public Object, public StateObject
{
  inline static std::string type {"checkbox"};
  explicit Checkbox(const StateObject &);
  Json::Value toJson() const override;
};

struct Radio : public Object, public StateObject
{
  inline static std::string type {"radio"};
  explicit Radio(const StateObject &);
  Json::Value toJson() const override;
};

struct TriState : public Object, public TriStateObject
{
  inline static std::string type {"tristate"};
  explicit TriState(const TriStateObject &);
  Json::Value toJson() const override;
};

struct Select : public Object, public StateObject
{
  inline static std::string type {"select"};
  explicit Select(const StateObject &);
  Json::Value toJson() const override;
};

#endif  // NONBIRI_CORE_OBJECT_H_