#include <core/extension/extension.h>

Extension::Extension()
{
#ifdef EXTENSION_DOMAIN_NAME
  domain = EXTENSION_DOMAIN_NAME;
#endif

#ifdef EXTENSION_BASE_URL
  baseUrl = EXTENSION_BASE_URL;
#endif

  if (baseUrl.back() == '/')
    baseUrl.pop_back();

#ifdef EXTENSION_NAME
  name = EXTENSION_NAME;
#endif

#ifdef EXTENSION_LANGUAGE
  language = EXTENSION_LANGUAGE;
#endif

#ifdef EXTENSION_VERSION
  version = EXTENSION_VERSION;
#endif

#ifdef EXTENSION_IS_NSFW
  isNsfw = true;
#endif
}

std::string Extension::latestsSelector() const
{
  return NULL;
}

std::string Extension::latestsNextSelector() const
{
  return NULL;
}

std::shared_ptr<Http::Response> Extension::latestsRequest(int) const
{
  return NULL;
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> Extension::parseLatestEntries(const Http::Response &) const
{
  ErrNotImplemented;
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> Extension::parseLatestEntries(HTML &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Manga_t> Extension::parseLatestEntry(Element &) const
{
  ErrNotImplemented;
}

std::string Extension::searchMangaSelector() const
{
  return NULL;
}

std::string Extension::searchMangaNextSelector() const
{
  return NULL;
}

std::shared_ptr<Http::Response> Extension::searchMangaRequest(
  int, const std::string &, const std::vector<std::pair<std::string, std::string>> &) const
{
  return nullptr;
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> Extension::parseSearchEntries(const Http::Response &) const
{
  ErrNotImplemented;
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> Extension::parseSearchEntries(HTML &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Manga_t> Extension::parseSearchEntry(Element &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Http::Response> Extension::mangaRequest(const std::string &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Manga_t> Extension::parseManga(const Http::Response &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Manga_t> Extension::parseManga(HTML &) const
{
  ErrNotImplemented;
}

std::string Extension::chaptersSelector() const
{
  return NULL;
}

std::shared_ptr<Http::Response> Extension::chaptersRequest(const Manga_t &) const
{
  return nullptr;
}

std::vector<std::shared_ptr<Chapter_t>> Extension::parseChapterEntries(const Manga_t &, const Http::Response &) const
{
  ErrNotImplemented;
}

std::vector<std::shared_ptr<Chapter_t>> Extension::parseChapterEntries(const Manga_t &, HTML &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Chapter_t> Extension::parseChapterEntry(const Manga_t &, Element &) const
{
  ErrNotImplemented;
}

std::shared_ptr<Http::Response> Extension::pagesRequest(const std::string &) const
{
  return nullptr;
}

std::vector<std::string> Extension::parsePages(const Http::Response &) const
{
  ErrNotImplemented;
}

std::vector<std::string> Extension::parsePages(HTML &) const
{
  ErrNotImplemented;
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

std::vector<std::shared_ptr<Chapter_t>> Extension::getChapters(const std::string &path) const
{
  const auto manga = getManga(path);
  return getChapters(*manga);
}

const std::vector<std::shared_ptr<const Filter::Filter>> &Extension::getFilters() const
{
  return filters.get();
}

const std::map<std::string, size_t> &Extension::getFiltersIndex() const
{
  return filters.getIndex();
}

Json::Value ExtensionInfo::toJson() const
{
  Json::Value root {};
  root["domain"] = domain;
  root["name"] = name;
  root["baseUrl"] = baseUrl;
  root["language"] = language;
  root["version"] = version;
  root["isNsfw"] = isNsfw;
  return root;
}