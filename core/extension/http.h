#ifndef NONBIRI_CORE_EXTENSION_HTTP_H_
#define NONBIRI_CORE_EXTENSION_HTTP_H_

#include <core/extension/extension.h>

class HttpExtension : public Extension
{
  friend class Manager;
  friend class Api;
  friend class Web;

public:
  HttpExtension();
  virtual ~HttpExtension() = default;

  virtual std::shared_ptr<Http::Response> latestsRequest(int page) const = 0;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(const Http::Response &response) const = 0;

  virtual std::shared_ptr<Http::Response> searchMangaRequest(
    int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const = 0;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(const Http::Response &response) const = 0;

  virtual std::shared_ptr<Http::Response> mangaRequest(const std::string &path) const = 0;
  virtual std::shared_ptr<Manga_t> parseManga(const Http::Response &response) const = 0;

  virtual std::shared_ptr<Http::Response> chaptersRequest(const Manga_t &manga) const = 0;
  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(const Manga_t &manga, const Http::Response &response) const = 0;

  virtual std::shared_ptr<Http::Response> pagesRequest(const std::string &path) const = 0;
  virtual std::vector<std::string> parsePages(const Http::Response &response) const = 0;

private:
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> getLatests(int page) const override;
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> searchManga(
    int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const override;
  std::shared_ptr<Manga_t> getManga(const std::string &path) const override;
  std::vector<std::shared_ptr<Chapter_t>> getChapters(const Manga_t &manga) const override;
  std::vector<std::string> getPages(const std::string &path) const override;
};

#endif  // NONBIRI_CORE_EXTENSION_HTTP_H_