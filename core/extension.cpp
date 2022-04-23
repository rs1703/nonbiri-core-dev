#include <iostream>

#include <core/extension.h>
#include <core/utils.h>

#define ElementPtr std::shared_ptr<Element>

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

#if USE_API == 1
  useApi = true;
#endif
}

Extension::~Extension()
{
  // std::cout << "Extension::~Extension()" << std::endl;
}

void Extension::init()
{
  try {
    auto filters = getFilters();
    for (const auto &filter : filters)
      filtersMap[filter.key] = filter;
  } catch (...) {
    // ignore
  }
}

std::string Extension::prependBaseUrl(const std::string &path)
{
  if (path.find(baseUrl) == 0 || path.find("http") == 0)
    return path;

  std::string result = baseUrl;
  if (path.front() != '/')
    result += '/';
  result += path;

  return result;
}

std::tuple<std::vector<MangaPtr>, bool> Extension::getLatests(int page)
{
  const std::string res = latestsRequest(std::max(page, 1));
  if (res.empty())
    throw std::runtime_error("No results");

  if (useApi) {
    const std::tuple<std::vector<Manga *>, bool> result = parseLatestEntries(res);
    return makeMangaEntries(result);
  }

  HTML html {res};
  try {
    const std::tuple<std::vector<Manga *>, bool> result = parseLatestEntries(html);
    return makeMangaEntries(result);
  } catch (...) {
    // ignore
  }

  const std::string selector = latestsSelector();
  const std::vector<ElementPtr> entries = html.select(selector);

  std::vector<MangaPtr> result;
  for (const ElementPtr &entry : entries) {
    const Manga *manga = parseLatestEntry(*entry);
    if (manga != nullptr)
      result.push_back(std::make_shared<Manga>(*manga));
  }

  const std::string nextSelector = latestsNextSelector();
  bool hasNext = false;

  if (!nextSelector.empty()) {
    const ElementPtr next = html.selectFirst(nextSelector);
    if (next)
      hasNext = next->isValid();
  }
  return std::make_tuple(result, hasNext);
}

std::tuple<std::vector<MangaPtr>, bool> Extension::searchManga(int page,
                                                               const std::string &query,
                                                               const std::vector<Filter> &filters)
{
  const std::string res = searchMangaRequest(std::max(page, 1), query, filters);
  if (res.empty())
    throw std::runtime_error("No results");

  if (useApi) {
    const std::tuple<std::vector<Manga *>, bool> result = parseSearchEntries(res);
    return makeMangaEntries(result);
  }

  HTML html {res};
  try {
    const std::tuple<std::vector<Manga *>, bool> result = parseSearchEntries(html);
    return makeMangaEntries(result);
  } catch (...) {
    // ignore
  }

  const std::string selector = searchMangaSelector();
  const std::vector<ElementPtr> entries = html.select(selector);
  std::vector<MangaPtr> result;

  for (const ElementPtr &entry : entries) {
    const Manga *manga = parseSearchEntry(*entry);
    if (manga != nullptr)
      result.push_back(std::make_shared<Manga>(*manga));
  }

  const std::string nextSelector = searchMangaNextSelector();
  bool hasNext = false;

  if (!nextSelector.empty()) {
    const ElementPtr next = html.selectFirst(nextSelector);
    if (next)
      hasNext = next->isValid();
  }
  return std::make_tuple(result, hasNext);
}

MangaPtr Extension::getManga(const std::string &path)
{
  const std::string uri {prependBaseUrl(path)};
  std::string cacheKey {id + path};

  const std::string res = http::get(uri);
  if (res.empty())
    throw std::runtime_error("No results");

  Manga *result;
  if (useApi) {
    result = parseManga(res);
  } else {
    HTML html {res};
    result = parseManga(html);
  }

  if (result == nullptr)
    throw std::runtime_error("No results");

  return std::make_shared<Manga>(*result);
}

std::vector<ChapterPtr> Extension::getChapters(Manga &manga)
{
  const std::string res = chaptersRequest(manga);
  if (res.empty())
    throw std::runtime_error("No results");

  if (useApi) {
    const std::vector<Chapter *> result = parseChapterEntries(manga, res);
    return makeChapterEntries(result);
  }

  HTML html {res};
  try {
    const std::vector<Chapter *> result = parseChapterEntries(manga, html);
    return makeChapterEntries(result);
  } catch (...) {
    // ignore
  }

  const std::string selector = chaptersSelector();
  const std::vector<ElementPtr> elements = html.select(selector);
  std::vector<ChapterPtr> result;

  for (const ElementPtr &element : elements) {
    const Chapter *entry = parseChapterEntry(manga, *element);
    if (entry != nullptr)
      result.push_back(std::make_shared<Chapter>(*entry));
  }

  return result;
}

std::vector<ChapterPtr> Extension::getChapters(const std::string &path)
{
  const MangaPtr manga = getManga(path);
  return getChapters(*manga);
}

std::vector<std::string> Extension::getPages(const std::string &path)
{
  const std::string res = pagesRequest(path);
  if (res.empty())
    throw std::runtime_error("No results");

  if (useApi)
    return parsePages(res);

  HTML html {res};
  return parsePages(html);
}

const std::map<std::string, FilterInfo> &Extension::getFiltersMap()
{
  return filtersMap;
}

std::tuple<std::vector<MangaPtr>, bool> Extension::makeMangaEntries(
  const std::tuple<std::vector<Manga *>, bool> &result)
{
  std::vector<MangaPtr> entries;
  for (const Manga *entry : std::get<0>(result))
    entries.push_back(std::make_shared<Manga>(*entry));
  return std::make_tuple(entries, std::get<1>(result));
}

std::vector<ChapterPtr> Extension::makeChapterEntries(const std::vector<Chapter *> &result)
{
  std::vector<ChapterPtr> entries;
  for (const Chapter *entry : result)
    entries.push_back(std::make_shared<Chapter>(*entry));
  return entries;
}
