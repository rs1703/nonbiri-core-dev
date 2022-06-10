#include <algorithm>

#include <core/extension/parsed.h>

using std::max;

ParsedExtension::ParsedExtension() : Extension() {}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> ParsedExtension::getLatests(int page) const
{
  const auto res = latestsRequest(max(page, 1));
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

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

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> ParsedExtension::searchManga(
  int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const
{
  const auto res = searchMangaRequest(max(page, 1), query, filters);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

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

std::shared_ptr<Manga_t> ParsedExtension::getManga(const std::string &path) const
{
  const auto res = mangaRequest(path);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  HTML html {res->body};
  auto result = parseManga(html);

  if (result == nullptr)
    throw std::runtime_error("No results");
  return result;
}

std::vector<std::shared_ptr<Chapter_t>> ParsedExtension::getChapters(const Manga_t &manga) const
{
  const auto res = chaptersRequest(manga);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

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

std::vector<std::string> ParsedExtension::getPages(const std::string &path) const
{
  const auto res = pagesRequest(path);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));

  HTML html {res->body};
  return parsePages(html);
}