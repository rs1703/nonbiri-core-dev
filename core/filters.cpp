#include <core/filters.h>

Json::Value Filter::toJson() const
{
  Json::Value root {};
  if (!name.empty())
    root["name"] = name;
  if (!key.empty())
    root["key"] = key;
  if (!type.empty())
    root["type"] = type;
  for (const auto &[k, v] : options)
    root["options"][k] = v;
  return root;
}

std::string Filter::toString() const
{
  Json::FastWriter writer {};
  return writer.write(toJson());
}