#include <algorithm>
#include <iostream>

#include <core/extension.h>

using std::max;

Extension::Extension()
{
#ifdef ID
  id = ID;
#endif

#ifdef BASE_URL
  baseUrl = BASE_URL;
#endif

  if (baseUrl.back() == '/')
    baseUrl.pop_back();

#ifdef NAME
  name = NAME;
#endif

#ifdef LANGUAGE
  language = LANGUAGE;
#endif

#ifdef VERSION
  version = VERSION;
#endif

#ifdef IS_NSFW
  isNsfw = true;
#endif

#ifdef USE_API
  useApi = true;
#endif
}

Extension::~Extension()
{
  // std::cout << "Extension::~Extension()" << std::endl;
}

std::string Extension::prependBaseUrl(const std::string &path) const
{
  if (path.find(baseUrl) == 0 || path.find("http") == 0)
    return path;

  auto result {baseUrl};
  if (path.front() != '/')
    result += '/';
  result += path;

  return result;
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> Extension::getLatests(int page) const
{
  const auto res = latestsRequest(max(page, 1));
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  if (useApi)
    return parseLatestEntries(*res);

  HTML html {res->body};
  try {
    return parseLatestEntries(html);
  } catch (...) {
    // ignore
  }

  const auto selector = latestsSelector();
  const auto entries  = html.select(selector);
  std::vector<std::shared_ptr<Manga_t>> result {};

  for (const auto &entry : entries) {
    const auto manga = parseLatestEntry(*entry);
    if (manga != nullptr)
      result.push_back(manga);
  }

  const auto nextSelector = latestsNextSelector();
  bool hasNext {};

  if (!nextSelector.empty()) {
    const auto next = html.selectFirst(nextSelector);
    if (next != nullptr)
      hasNext = next->isValid();
  }
  return std::make_tuple(result, hasNext);
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> Extension::searchManga(
  int page, const std::string &query, const std::vector<Filter::Pair> &filters) const
{
  const auto res = searchMangaRequest(max(page, 1), query, filters);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  if (useApi)
    return parseSearchEntries(*res);

  HTML html {res->body};
  try {
    return parseSearchEntries(html);
  } catch (...) {
    // ignore
  }

  const auto selector = searchMangaSelector();
  const auto entries  = html.select(selector);
  std::vector<std::shared_ptr<Manga_t>> result {};

  for (const auto &entry : entries) {
    const auto manga = parseSearchEntry(*entry);
    if (manga != nullptr)
      result.push_back(manga);
  }

  const auto nextSelector = searchMangaNextSelector();
  bool hasNext {};

  if (!nextSelector.empty()) {
    const auto next = html.selectFirst(nextSelector);
    if (next != nullptr)
      hasNext = next->isValid();
  }
  return std::make_tuple(result, hasNext);
}

std::shared_ptr<Manga_t> Extension::getManga(const std::string &path) const
{
  auto res = client.get(prependBaseUrl(path));
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  std::shared_ptr<Manga_t> result {nullptr};
  if (useApi) {
    result = parseManga(*res);
  } else {
    HTML html {res->body};
    result = parseManga(html);
  }

  if (result == nullptr)
    throw std::runtime_error("No results");

  return result;
}

std::vector<std::shared_ptr<Chapter_t>> Extension::getChapters(const Manga_t &manga) const
{
  const auto res = chaptersRequest(manga);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  if (useApi)
    return parseChapterEntries(manga, *res);

  HTML html {res->body};
  try {
    return parseChapterEntries(manga, html);
  } catch (...) {
    // ignore
  }

  const auto selector = chaptersSelector();
  const auto elements = html.select(selector);
  std::vector<std::shared_ptr<Chapter_t>> result {};

  for (const auto &element : elements) {
    const auto entry = parseChapterEntry(manga, *element);
    if (entry != nullptr)
      result.push_back(entry);
  }

  return result;
}

std::vector<std::shared_ptr<Chapter_t>> Extension::getChapters(const std::string &path) const
{
  const auto manga = getManga(path);
  return getChapters(*manga);
}

std::vector<std::string> Extension::getPages(const std::string &path) const
{
  const auto res = pagesRequest(path);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  if (useApi)
    return parsePages(*res);

  HTML html {res->body};
  return parsePages(html);
}

const std::map<std::string, const Filter> &Extension::getFilters() const
{
  return filters.get();
}