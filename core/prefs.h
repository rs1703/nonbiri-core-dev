#ifndef NONBIRI_CORE_PREFS_H_
#define NONBIRI_CORE_PREFS_H_

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <core/object.h>
#include <json/json.h>

class Prefs;

namespace Pref
{
template<typename T>
using OnChangeHandler = std::function<void(const T *)>;

template<typename T>
class Handler
{
  friend class ::Prefs;

  OnChangeHandler<T> onChange {nullptr};

public:
  explicit Handler() = default;
  explicit Handler(OnChangeHandler<T>);
  virtual ~Handler() = default;
};

struct Input : public ::Input, public Handler<InputOption>
{
  using ::Input::Input;
  explicit Input(const Json::Value &);
  explicit Input(const InputOption &, OnChangeHandler<InputOption>);
};

struct Checkbox : public ::Checkbox, public Handler<StateObject>
{
  using ::Checkbox::Checkbox;
  explicit Checkbox(const Json::Value &);
  explicit Checkbox(const StateObject &, OnChangeHandler<StateObject>);
};

struct Radio : public ::Radio, public Handler<StateObject>
{
  using ::Radio::Radio;
  explicit Radio(const Json::Value &);
  explicit Radio(const StateObject &, OnChangeHandler<StateObject>);
};

struct TriState : public ::TriState, public Handler<TriStateObject>
{
  using ::TriState::TriState;
  explicit TriState(const Json::Value &);
  explicit TriState(const TriStateObject &, OnChangeHandler<TriStateObject>);
};

struct Select : public ::Select, public Handler<StateObject>
{
  using ::Select::Select;
  explicit Select(const Json::Value &);
  explicit Select(const StateObject &, OnChangeHandler<StateObject>);
};
}  // namespace Pref

class Prefs
{
  friend class Manager;
  friend class Api;

  std::vector<std::shared_ptr<Object>> m_objects {};
  std::map<std::string, size_t> m_index {};
  std::string path {};

public:
  explicit Prefs() = delete;
  explicit Prefs(const std::string &domain);
  virtual ~Prefs() = default;

  template<typename T>
  const T *get(const std::string &key) const;

  void add(Text *);
  void add(InputOption *);
  void add(TriStateObject *);
  void add(StateObject *);

  const std::vector<std::shared_ptr<Object>> &list() const;
  const std::map<std::string, size_t> &index() const;

private:
  void set(Text *);
  void set(InputOption *);
  void set(TriStateObject *);
  void set(StateObject *);

  void update(const Json::Value &);
  void save() const;

  template<typename T, typename U>
  void onChange(U *newObj, const U *oldObj);

  Json::Value toJson() const;
};

#endif  // NONBIRI_CORE_PREFS_H_