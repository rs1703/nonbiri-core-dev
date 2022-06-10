#include <algorithm>

#include <core/extension/http.h>

using std::max;

HttpExtension::HttpExtension() : Extension() {}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> HttpExtension::getLatests(int page) const
{
  const auto res = latestsRequest(max(page, 1));
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));
  return parseLatestEntries(*res);
}

std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> HttpExtension::searchManga(
  int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const
{
  const auto res = searchMangaRequest(max(page, 1), query, filters);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));
  return parseSearchEntries(*res);
}

std::shared_ptr<Manga_t> HttpExtension::getManga(const std::string &path) const
{
  const auto res = mangaRequest(path);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));
  auto manga = parseManga(*res);
  if (manga == nullptr)
    throw std::runtime_error("No results");
  return manga;
}

std::vector<std::shared_ptr<Chapter_t>> HttpExtension::getChapters(const Manga_t &manga) const
{
  const auto res = chaptersRequest(manga);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));
  return parseChapterEntries(manga, *res);
}

std::vector<std::string> HttpExtension::getPages(const std::string &path) const
{
  const auto res = pagesRequest(path);
  if (res->body.empty())
    throw std::runtime_error("No results: " + std::to_string(res->statusCode));
  return parsePages(*res);
}