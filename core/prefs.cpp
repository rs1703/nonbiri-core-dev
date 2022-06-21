#include <filesystem>
#include <fstream>

#include <core/prefs.h>

namespace fs = std::filesystem;
const std::string dir {"prefs"};

namespace Pref
{
template<typename T>
Handler<T>::Handler(OnChangeHandler<T> h) : onChange(h)
{
}

InputOption createInputOption(const Json::Value &root)
{
  return {
    root["key"].asString(),
    root["title"].asString(),
    root["description"].asString(),
    root["value"].asString(),
    root["defaultValue"].asString(),
  };
}

StateOption createStateOption(const Json::Value &root)
{
  return {
    root["key"].asString(),
    root["title"].asString(),
    root["value"].asString(),
    root["state"].asInt(),
    root["defaultState"].asInt(),
  };
}

StateObject createStateObject(const Json::Value &root)
{
  std::vector<StateOption> options {};
  if (root.isMember("options")) {
    for (const auto &option : root["options"])
      options.push_back(createStateOption(option));
  }
  return {
    root["key"].asString(),
    root["title"].asString(),
    root["description"].asString(),
    options,
  };
}

TriStateObject createTriStateObject(const Json::Value &root)
{
  std::vector<StateOption> options {};
  if (root.isMember("options")) {
    for (const auto &option : root["options"])
      options.push_back(createStateOption(option));
  }
  return {
    root["key"].asString(),
    root["excludedKey"].asString(),
    root["title"].asString(),
    root["description"].asString(),
    options,
  };
}

Input::Input(const Json::Value &root) : ::Input(createInputOption(root)) {}
Input::Input(const InputOption &o, OnChangeHandler<InputOption> h) : ::Input(o), Handler(h) {}

Checkbox::Checkbox(const Json::Value &root) : ::Checkbox(createStateObject(root)) {}
Checkbox::Checkbox(const StateObject &o, OnChangeHandler<StateObject> h) : ::Checkbox(o), Handler(h) {}

Radio::Radio(const Json::Value &root) : ::Radio(createStateObject(root)) {}
Radio::Radio(const StateObject &o, OnChangeHandler<StateObject> h) : ::Radio(o), Handler(h) {}

TriState::TriState(const Json::Value &root) : ::TriState(createTriStateObject(root)) {}
TriState::TriState(const TriStateObject &o, OnChangeHandler<TriStateObject> h) : ::TriState(o), Handler(h) {}

Select::Select(const Json::Value &root) : ::Select(createStateObject(root)) {}
Select::Select(const StateObject &o, OnChangeHandler<StateObject> h) : ::Select(o), Handler(h) {}
}  // namespace Pref

void copyStateValues(StateObject *object, const StateObject *_object)
{
  for (auto &option : object->options) {
    for (const auto &_option : _object->options) {
      if ((!option.key.empty() && option.key == _option.key) || option.title == _option.title) {
        option.state = _option.state;
        break;
      }
    }
  }
}

Prefs::Prefs(const std::string &domain)
{
  if (!fs::exists(dir))
    fs::create_directory(dir);

  path = (fs::path(dir) / (domain + ".json"));
  if (!fs::exists(fs::path(path)))
    return;

  std::ifstream file {path};
  if (!file.is_open())
    return;

  Json::Value root {};
  file >> root;
  file.close();

  for (const auto &data : root) {
    const std::string type = data["type"].asString();
    if (type == Text::type) {
      add(new Text(data["key"].asString(), data["value"].asString()));
    } else if (type == Pref::Input::type) {
      add(new Pref::Input(data));
    } else if (type == Pref::Checkbox::type) {
      add(new Pref::Checkbox(data));
    } else if (type == Pref::Radio::type) {
      add(new Pref::Radio(data));
    } else if (type == Pref::TriState::type) {
      add(new Pref::TriState(data));
    } else if (type == Pref::Select::type) {
      add(new Pref::Select(data));
    }
  }
}

#define DefineGetter(type) \
  template<> \
  const type *Prefs::get<type>(const std::string &key) const \
  { \
    const auto it = m_index.find(key); \
    if (it == m_index.end()) \
      return nullptr; \
    return dynamic_cast<const type *>(m_objects[it->second].get()); \
  }

DefineGetter(InputOption);
DefineGetter(StateObject);
DefineGetter(TriStateObject);
DefineGetter(Text);
DefineGetter(Input);
DefineGetter(Checkbox);
DefineGetter(Radio);
DefineGetter(TriState);
DefineGetter(Select);

void Prefs::add(Text *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->_key);
  if (it == m_index.end()) {
    m_objects.push_back(std::shared_ptr<Text>(object));
    m_index[object->_key] = m_objects.size() - 1;
    save();
  } else {
    object->value = get<Text>(object->key)->value;
    set(object);
  }
}

void Prefs::set(Text *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->_key);
  if (it == m_index.end())
    return;

  m_objects[it->second].reset(object);
  save();
}

void Prefs::add(InputOption *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->key);
  if (it == m_index.end()) {
    m_objects.push_back(std::shared_ptr<Input>((Input *)object));
    m_index[object->key] = m_objects.size() - 1;
    save();
  } else {
    object->value = get<Input>(object->key)->value;
    set(object);
  }
}

void Prefs::set(InputOption *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->key);
  if (it == m_index.end())
    return;

  auto input = (Input *)object;
  onChange<InputOption>(input, get<Input>(object->key));
  m_objects[it->second].reset(input);
  save();
}

void Prefs::add(StateObject *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->key);
  if (it == m_index.end()) {
    m_objects.push_back(std::shared_ptr<Checkbox>((Checkbox *)object));
    m_index[object->key] = m_objects.size() - 1;
    save();
  } else {
    copyStateValues(object, get<StateObject>(object->key));
    set(object);
  }
}

void Prefs::set(StateObject *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->key);
  if (it == m_index.end())
    return;

  if (auto ptr = get<Checkbox>(object->key); ptr != nullptr) {
    onChange<StateObject>((Checkbox *)object, ptr);
    m_objects[it->second].reset((Checkbox *)object);
  } else if (auto ptr = get<Radio>(object->key); ptr != nullptr) {
    onChange<StateObject>((Radio *)object, ptr);
    m_objects[it->second].reset((Radio *)object);
  } else if (auto ptr = get<Select>(object->key); ptr != nullptr) {
    onChange<StateObject>((Select *)object, ptr);
    m_objects[it->second].reset((Select *)object);
  }
  save();
}

void Prefs::add(TriStateObject *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->key);
  if (it == m_index.end()) {
    m_objects.push_back(std::shared_ptr<Object>((TriState *)object));
    m_index[object->key] = m_objects.size() - 1;
    save();
  } else {
    auto _object = get<TriState>(object->key);
    for (auto &option : object->options) {
      for (const auto &_option : _object->options) {
        if ((!option.key.empty() && option.key == _option.key) || option.title == _option.title) {
          option.state = _option.state;
          break;
        }
      }
    }
    set(object);
  }
}

void Prefs::set(TriStateObject *object)
{
  if (object == nullptr)
    return;

  auto it = m_index.find(object->key);
  if (it == m_index.end())
    return;

  onChange<TriStateObject>((TriState *)object, get<TriState>(object->key));
  m_objects[it->second].reset((TriState *)object);
  save();
}

const std::vector<std::shared_ptr<Object>> &Prefs::list() const
{
  return m_objects;
}

const std::map<std::string, size_t> &Prefs::index() const
{
  return m_index;
}

template<typename T, typename U>
void Prefs::onChange(U *newObj, const U *oldObj)
{
  auto handler = dynamic_cast<Pref::Handler<T> *>(newObj);
  if (handler != nullptr) {
    auto _handler = dynamic_cast<Pref::Handler<T> *>(const_cast<U *>(oldObj));
    if (handler->onChange == nullptr && _handler != nullptr)
      handler->onChange = _handler->onChange;
    handler->onChange(newObj);
  }
}

void Prefs::update(const Json::Value &root)
{
  for (const auto &data : root) {
    const std::string type = data["type"].asString();
    if (type == Text::type) {
      set(new Text(data["key"].asString(), data["value"].asString()));
    } else if (type == Pref::Input::type) {
      set(new Pref::Input(data));
    } else if (type == Pref::Checkbox::type) {
      set(new Pref::Checkbox(data));
    } else if (type == Pref::Radio::type) {
      set(new Pref::Radio(data));
    } else if (type == Pref::TriState::type) {
      set(new Pref::TriState(data));
    } else if (type == Pref::Select::type) {
      set(new Pref::Select(data));
    }
  }
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

Json::Value Prefs::toJson() const
{
  Json::Value root {};
  for (const auto &object : m_objects)
    root.append(object->toJson());
  return root;
}
