#ifndef NONBIRI_CORE_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_H_

#include <atomic>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <core/filters.h>
#include <core/http/http.h>
#include <core/models.h>
#include <core/parser.h>
#include <core/prefs.h>
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

class Extension : public ExtensionInfo
{
  friend class Manager;
  friend class Api;
  friend class Web;

  std::atomic_bool hasUpdate {};

protected:
  Http::Client client {};
  Filters filters {};

public:
  bool useApi {};

  Extension();
  ~Extension();

  virtual std::string latestsSelector() const
  {
    return NULL;
  }

  virtual std::string latestsNextSelector() const
  {
    return NULL;
  }

  virtual std::shared_ptr<Http::Response> latestsRequest(int page) const
  {
    return NULL;
  }

  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(
    const Http::Response &response) const
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

  virtual std::shared_ptr<Http::Response> searchMangaRequest(
    int page, const std::string &query, const std::vector<Filter::Pair> &filters) const
  {
    return NULL;
  }

  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(
    const Http::Response &response) const
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

  virtual std::shared_ptr<Http::Response> mangaRequest(const std::string &path) const
  {
    ErrNotImplemented;
  }

  virtual std::shared_ptr<Manga_t> parseManga(const Http::Response &response) const
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

  virtual std::shared_ptr<Http::Response> chaptersRequest(const Manga_t &manga) const
  {
    return NULL;
  }

  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(
    const Manga_t &manga, const Http::Response &response) const
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

  virtual std::shared_ptr<Http::Response> pagesRequest(const std::string &path) const
  {
    return NULL;
  }

  virtual std::vector<std::string> parsePages(const Http::Response &response) const
  {
    ErrNotImplemented;
  }

  virtual std::vector<std::string> parsePages(HTML &html) const
  {
    ErrNotImplemented;
  }

  virtual Prefs *getPrefs() const
  {
    ErrNotImplemented;
  }

protected:
  std::string prependBaseUrl(const std::string &path) const;

private:
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> getLatests(int page = 1) const;
  std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> searchManga(
    int page = 1, const std::string &query = "", const std::vector<Filter::Pair> &filters = {}) const;
  std::shared_ptr<Manga_t> getManga(const std::string &path) const;
  std::vector<std::shared_ptr<Chapter_t>> getChapters(const Manga_t &manga) const;
  std::vector<std::shared_ptr<Chapter_t>> getChapters(const std::string &path) const;
  std::vector<std::string> getPages(const std::string &path) const;

  const std::unordered_map<std::string, const Filter> &getFilters() const;
};

typedef Extension *(*create_t)();
#define RegisterExtension(extension) \
  EXPORT_API extension *create() \
  { \
    return new extension(); \
  }

#endif  // NONBIRI_CORE_EXTENSION_H_