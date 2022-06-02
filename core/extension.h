#ifndef NONBIRI_CORE_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_H_

#include <atomic>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <core/filters.h>
#include <core/http.h>
#include <core/models.h>
#include <core/parser.h>
#include <core/utility.h>

#define ErrNotImplemented throw std::runtime_error("Not implemented")

struct ExtensionInfo
{
  std::string id {};
  std::string baseUrl {};
  std::string name {};
  std::string language {};
  std::string version {};
  bool isNsfw {};
};

class Manager;
class Api;
class Web;

class Extension : public ExtensionInfo
{
  friend class Manager;
  friend class Api;
  friend class Web;

  std::map<std::string, Filter> filtersMap {};
  std::atomic_bool hasUpdate {};

protected:
  HttpClient http;

public:
  bool useApi {};

  Extension();
  ~Extension();

  void init();

  virtual std::string latestsSelector() const
  {
    return NULL;
  }

  virtual std::string latestsNextSelector() const
  {
    return NULL;
  }

  virtual std::string latestsRequest(int page) const
  {
    return NULL;
  }

  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(const std::string &response) const
  {
    ErrNotImplemented;
  }

  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(HTML &html) const
  {
    ErrNotImplemented;
  }

  virtual std::shared_ptr<Manga_t> parseLatestEntry(Element &element) const
  {
    ErrNotImplemented;
  }

  virtual std::string searchMangaSelector() const
  {
    return NULL;
  }

  virtual std::string searchMangaNextSelector() const
  {
    return NULL;
  }

  virtual std::string searchMangaRequest(int page, const std::string &query, const std::vector<FilterKV> &filters) const
  {
    return NULL;
  }

  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(const std::string &response) const
  {
    ErrNotImplemented;
  }

  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(HTML &html) const
  {
    ErrNotImplemented;
  }

  virtual std::shared_ptr<Manga_t> parseSearchEntry(Element &element) const
  {
    ErrNotImplemented;
  }

  virtual std::shared_ptr<Manga_t> parseManga(const std::string &response) const
  {
    ErrNotImplemented;
  }

  virtual std::shared_ptr<Manga_t> parseManga(HTML &html) const
  {
    ErrNotImplemented;
  }

  virtual std::string chaptersSelector() const
  {
    return NULL;
  }

  virtual std::string chaptersRequest(const Manga_t &manga) const
  {
    return NULL;
  }

  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(const Manga_t &manga,
                                                                      const std::string &response) const
  {
    ErrNotImplemented;
  }

  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(const Manga_t &manga, HTML &html) const
  {
    ErrNotImplemented;
  }

  virtual std::shared_ptr<Chapter_t> parseChapterEntry(const Manga_t &manga, Element &element) const
  {
    ErrNotImplemented;
  }

  virtual std::string pagesRequest(const std::string &path) const
  {
    return NULL;
  }

  virtual std::vector<std::string> parsePages(const std::string &response) const
  {
    ErrNotImplemented;
  }

  virtual std::vector<std::string> parsePages(HTML &html) const
  {
    ErrNotImplemented;
  }

  virtual const std::vector<Filter> &getFilters() const
  {
    ErrNotImplemented;
  }

protected:
  std::string prependBaseUrl(const std::string &path) const;

private:
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> getLatests(int page = 1);
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> searchManga(int page = 1,
                                                                      const std::string &query = "",
                                                                      const std::vector<FilterKV> &filters = {});
  std::shared_ptr<Manga_t> getManga(const std::string &path);
  std::vector<std::shared_ptr<Chapter_t>> getChapters(const Manga_t &manga);
  std::vector<std::shared_ptr<Chapter_t>> getChapters(const std::string &path);
  std::vector<std::string> getPages(const std::string &path);

  const std::map<std::string, Filter> &getFiltersMap();
};

typedef Extension *(*create_t)();
#define RegisterExtension(extension) \
  EXPORT_API extension *create() \
  { \
    auto ext = new extension(); \
    ext->init(); \
\
    return ext; \
  }

#endif  // NONBIRI_CORE_EXTENSION_H_