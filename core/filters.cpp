#include <core/filters.h>

Json::Value Filter::toJson() const
{
  Json::Value root {};
  root["name"] = name;
  root["key"] = key;
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