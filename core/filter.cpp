#include <core/filter.h>

namespace Filter
{
Json::Value Option::toJson() const
{
  Json::Value json;
  json["key"] = key;
  json["value"] = value;
  if (defaultState > State::Disabled)
    json["defaultState"] = defaultState;
  return json;
}

Filter::Filter(const Info &info) : Info {info} {}

Json::Value Filter::toJson() const
{
  Json::Value root {};
  root["key"] = key;

  if (hidden) {
    root["isHidden"] = hidden;
  } else {
    root["title"] = title;

    if (!description.empty())
      root["description"] = description;

    root["options"] = Json::Value(Json::arrayValue);
    for (const auto &option : options)
      root["options"].append(option.toJson());
  }
  return root;
}

Hidden::Hidden(const std::string &key) : Filter {Info {.key = key, .hidden = true}} {}

Checkbox::Checkbox(const Info &info) : Filter {info} {}

Json::Value Checkbox::toJson() const
{
  Json::Value root = Filter::toJson();
  root["type"] = "checkbox";
  return root;
}

ExcludableCheckbox::ExcludableCheckbox(const std::string &excludedKey, const Info &info) : Checkbox {info}, excludedKey {excludedKey} {}

Json::Value ExcludableCheckbox::toJson() const
{
  Json::Value root = Checkbox::toJson();
  root["excludedKey"] = excludedKey;
  return root;
}

Radio::Radio(const Info &info) : Filter {info} {}

Json::Value Radio::toJson() const
{
  Json::Value root = Filter::toJson();
  root["type"] = "radio";
  return root;
}

Select::Select(const Info &info) : Filter {info} {}

Json::Value Select::toJson() const
{
  Json::Value root = Filter::toJson();
  root["type"] = "select";
  return root;
}

void Filters::add(const Filter *filter)
{
  filters.push_back(std::shared_ptr<const Filter>(filter));
  index[filter->key] = filters.size() - 1;
}

const std::vector<std::shared_ptr<const Filter>> &Filters::get() const
{
  return filters;
}

const std::map<std::string, size_t> &Filters::getIndex() const
{
  return index;
}
}  // namespace Filter