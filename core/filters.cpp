#include <core/filters.h>

namespace Filter
{
Hidden::Hidden(const std::string &key) : Object {key}, key {key} {}
Json::Value Hidden::toJson() const
{
  return Json::Value {};
}
}  // namespace Filter

void Filters::add(const Object *filter)
{
  m_filters.push_back(std::shared_ptr<const Object> {filter});
  m_index[filter->_key] = m_filters.size() - 1;
}

const std::vector<std::shared_ptr<const Object>> &Filters::list() const
{
  return m_filters;
}

const std::map<std::string, size_t> &Filters::index() const
{
  return m_index;
}

Json::Value Filters::toJson() const
{
  Json::Value root {};
  for (const auto &filter : m_filters) {
    if (reinterpret_cast<const Filter::Hidden *>(filter.get()) == nullptr)
      root.append(filter->toJson());
  }
  return root;
}