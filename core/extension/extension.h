#ifndef NONBIRI_CORE_EXTENSION_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_EXTENSION_H_

#include <atomic>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <core/filter.h>
#include <core/http.h>
#include <core/models.h>
#include <core/parser.h>
#include <core/pref.h>
#include <core/utility.h>
#include <json/json.h>

#define ErrNotImplemented throw std::runtime_error("Not implemented")

struct ExtensionInfo
{
  friend class Extension;
  friend class Manager;
  friend class Api;
  friend class Web;

  std::string id {};
  std::string baseUrl {};
  std::string name {};
  std::string language {};
  std::string version {};
  bool isNsfw {};

private:
  Json::Value toJson() const;
};

class Extension : public ExtensionInfo
{
  friend class Manager;
  friend class Api;
  friend class Web;

  std::atomic_bool hasUpdate {};

protected:
  Http::Client client {};
  Filter::Filters filters {};

public:
  Extension();
  virtual ~Extension() = default;

  virtual std::string latestsSelector() const;
  virtual std::string latestsNextSelector() const;
  virtual std::shared_ptr<Http::Response> latestsRequest(int page) const;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(const Http::Response &response) const;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseLatestEntries(HTML &html) const;
  virtual std::shared_ptr<Manga_t> parseLatestEntry(Element &element) const;

  virtual std::string searchMangaSelector() const;
  virtual std::string searchMangaNextSelector() const;

  virtual std::shared_ptr<Http::Response> searchMangaRequest(
    int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(const Http::Response &response) const;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> parseSearchEntries(HTML &html) const;
  virtual std::shared_ptr<Manga_t> parseSearchEntry(Element &element) const;

  virtual std::shared_ptr<Http::Response> mangaRequest(const std::string &path) const;
  virtual std::shared_ptr<Manga_t> parseManga(const Http::Response &response) const;
  virtual std::shared_ptr<Manga_t> parseManga(HTML &html) const;

  virtual std::string chaptersSelector() const;
  virtual std::shared_ptr<Http::Response> chaptersRequest(const Manga_t &manga) const;
  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(const Manga_t &manga, const Http::Response &response) const;
  virtual std::vector<std::shared_ptr<Chapter_t>> parseChapterEntries(const Manga_t &manga, HTML &html) const;
  virtual std::shared_ptr<Chapter_t> parseChapterEntry(const Manga_t &manga, Element &element) const;

  virtual std::shared_ptr<Http::Response> pagesRequest(const std::string &path) const;
  virtual std::vector<std::string> parsePages(const Http::Response &response) const;
  virtual std::vector<std::string> parsePages(HTML &html) const;

  virtual Pref::Prefs *prefs() const;

protected:
  std::string prependBaseUrl(const std::string &path) const;

private:
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> getLatests(int page) const = 0;
  virtual std::tuple<std::vector<std::shared_ptr<Manga_t>>, bool> searchManga(
    int page, const std::string &query, const std::vector<std::pair<std::string, std::string>> &filters) const = 0;
  virtual std::shared_ptr<Manga_t> getManga(const std::string &path) const                                     = 0;
  virtual std::vector<std::shared_ptr<Chapter_t>> getChapters(const Manga_t &manga) const                      = 0;
  virtual std::vector<std::shared_ptr<Chapter_t>> getChapters(const std::string &path) const;
  virtual std::vector<std::string> getPages(const std::string &path) const = 0;

  const std::vector<std::shared_ptr<const Filter::Filter>> &getFilters() const;
  const std::map<std::string, size_t> &getFiltersIndex() const;
};

typedef Extension *(*create_t)();
#define RegisterExtension(extension) \
  EXPORT_API extension *create() \
  { \
    return new extension(); \
  }

#define WithPrefs(extension) \
  Pref::Prefs *extension::prefs() const \
  { \
    return const_cast<Pref::Prefs *>((const Pref::Prefs *)this); \
  }

#endif  // NONBIRI_CORE_EXTENSION_EXTENSION_H_