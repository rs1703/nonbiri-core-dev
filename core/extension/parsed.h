#ifndef NONBIRI_CORE_EXTENSION_PARSED_H_
#define NONBIRI_CORE_EXTENSION_PARSED_H_

#include <core/extension/extension.h>

class ParsedExtension : public Extension
{
  friend class Manager;
  friend class Api;
  friend class Web;

public:
  ParsedExtension();
  virtual ~ParsedExtension() = default;

  virtual std::string latestsSelector() const                                                          = 0;
  virtual std::string latestsNextSelector() const                                                      = 0;
  virtual std::shared_ptr<Http::Response> latestsRequest(int page) const                               = 0;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(HTML &html) const = 0;
  virtual std::shared_ptr<Manga_t> parseLatestEntry(Element &element) const                            = 0;

  virtual std::string searchMangaSelector() const     = 0;
  virtual std::string searchMangaNextSelector() const = 0;

  virtual std::shared_ptr<Http::Response> searchMangaRequest(
    int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const = 0;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(HTML &html) const         = 0;
  virtual std::shared_ptr<Manga_t> parseSearchEntry(Element &element) const                                    = 0;

  virtual std::shared_ptr<Http::Response> mangaRequest(const std::string &path) const = 0;
  virtual std::shared_ptr<Manga_t> parseManga(HTML &html) const                       = 0;

  virtual std::string chaptersSelector() const                                                                = 0;
  virtual std::shared_ptr<Http::Response> chaptersRequest(const Manga_t &manga) const                         = 0;
  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(const Manga_t &manga, HTML &html) const = 0;
  virtual std::shared_ptr<Chapter_t> parseChapterEntry(const Manga_t &manga, Element &element) const          = 0;

  virtual std::shared_ptr<Http::Response> pagesRequest(const std::string &path) const = 0;
  virtual std::vector<std::string> parsePages(HTML &html) const                       = 0;

private:
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> getLatests(int page) const override;
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> searchManga(
    int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const override;
  std::shared_ptr<Manga_t> getManga(const std::string &path) const override;
  std::vector<std::shared_ptr<Chapter_t>> getChapters(const Manga_t &manga) const override;
  std::vector<std::string> getPages(const std::string &path) const override;
};

#endif  // NONBIRI_CORE_EXTENSION_PARSED_H_