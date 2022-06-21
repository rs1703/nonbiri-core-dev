#ifndef NONBIRI_CORE_FILTERS_H_
#define NONBIRI_CORE_FILTERS_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <core/object.h>
#include <json/json.h>

namespace Filter
{
struct Hidden : public Object
{
  const std::string key {};
  explicit Hidden(const std::string &key);
  Json::Value toJson() const override;
};
}  // namespace Filter

class Filters
{
  friend class Manager;
  friend class Api;

  std::vector<std::shared_ptr<const Object>> m_filters {};
  std::map<std::string, size_t> m_index {};

public:
  explicit Filters() = default;
  virtual ~Filters() = default;

  void add(const Object *);

  const std::vector<std::shared_ptr<const Object>> &list() const;
  const std::map<std::string, size_t> &index() const;

private:
  Json::Value toJson() const;
};

#endif  // NONBIRI_CORE_FILTERS_H_