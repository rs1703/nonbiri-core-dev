#include <core/filters.h>

Json::Value Filter::Pair::toJson() const
{
  Json::Value json;
  json["key"]   = key;
  json["value"] = value;
  return json;
}

Json::Value Filter::Option::toJson() const
{
  Json::Value json;
  json["key"]   = key;
  json["value"] = value;
  if (defaultOption)
    json["defaultOption"] = defaultOption;
  return json;
}

Filter::Filter(const Filter &filter) :
  key(filter.key),
  title(filter.title),
  description(filter.description),
  options(filter.options),
  type(filter.type) {};

Filter::Filter(const string &key, const string &title, const std::vector<Option> &options, const string &type) :
  key {key},
  title {title},
  options {options},
  type {type}
{
}

Filter::Filter(const string &key,
  const string &title,
  const string &description,
  const std::vector<Option> &options,
  const string &type) :
  key {key},
  title {title},
  description {description},
  options {options},
  type {type}
{
}

Json::Value Filter::toJson() const
{
  Json::Value root {};
  root["key"]   = key;
  root["title"] = title;
  root["type"]  = type;

  if (!description.empty())
    root["description"] = description;

  root["options"] = Json::Value(Json::arrayValue);
  for (const auto &option : options)
    root["options"].append(option.toJson());

  return root;
}

std::string Filter::toString() const
{
  Json::FastWriter writer {};
  return writer.write(toJson());
}

void Filters::add(const Filter *filter)
{
  filters.emplace(filter->key, std::shared_ptr<const Filter>(filter));
}

void Filters::remove(const Filter *filter)
{
  filters.erase(filter->key);
}

void Filters::remove(const std::string &key)
{
  filters.erase(key);
}

const std::unordered_map<std::string, std::shared_ptr<const Filter>> &Filters::get() const
{
  return filters;
}

Checkbox::Checkbox(const string &key, const string &title, const std::vector<Option> &options) :
  Filter {key, title, options, "checkbox"}
{
}

Checkbox::Checkbox(
  const string &key, const string &title, const string &description, const std::vector<Option> &options) :
  Filter {key, title, description, options, "checkbox"}
{
}

ExcludableCheckbox::ExcludableCheckbox(
  const string &key, const string &excludedKey, const string &title, const std::vector<Option> &options) :
  Filter {key, title, options, "checkbox"},
  excludedKey {excludedKey}
{
}

ExcludableCheckbox::ExcludableCheckbox(const string &key,
  const string &excludedKey,
  const string &title,
  const string &description,
  const std::vector<Option> &options) :
  Filter {key, title, description, options, "checkbox"},
  excludedKey {excludedKey}
{
}

Json::Value ExcludableCheckbox::toJson() const
{
  Json::Value root    = Filter::toJson();
  root["excludedKey"] = excludedKey;
  return root;
}

Radio::Radio(const string &key, const string &title, const std::vector<Option> &options) :
  Filter {key, title, options, "radio"}
{
}

Radio::Radio(const string &key, const string &title, const string &description, const std::vector<Option> &options) :
  Filter {key, title, description, options, "radio"}
{
}

Select::Select(const string &key, const string &title, const std::vector<Option> &options) :
  Filter {key, title, options, "select"}
{
}

Select::Select(const string &key, const string &title, const string &description, const std::vector<Option> &options) :
  Filter {key, title, description, options, "select"}
{
}