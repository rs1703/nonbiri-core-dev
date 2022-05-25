#ifndef NONBIRI_CORE_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_H_

#include <atomic>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <core/filters.h>
#include <core/models.h>
#include <core/parser.h>

#define ErrNotImplemented throw std::runtime_error("Not implemented")

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define EXPORT_API extern "C" __declspec(dllexport)
#else
#  define EXPORT_API extern "C"
#endif

#define RegisterExtension(extension) \
  EXPORT_API extension *create() \
  { \
    auto ext = new extension(); \
    ext->init(); \
\
    return ext; \
  }

struct ExtensionInfo
{
  std::string id {};
  std::string baseUrl {};
  std::string name {};
  std::string language {};
  std::string version {};
  bool isNsfw {false};
};

class Manager;
class Api;
class Web;

class Extension : public ExtensionInfo
{
  friend class Manager;
  friend class Api;
  friend class Web;

  std::map<std::string, Filter> filtersMap;
  std::atomic_bool hasUpdate {false};

public:
  bool useApi {false};

  Extension();
  ~Extension();

  void init();

  virtual std::string latestsSelector()
  {
    return NULL;
  }

  virtual std::string latestsNextSelector()
  {
    return NULL;
  }

  virtual std::string latestsRequest(int page)
  {
    return NULL;
  }

  virtual std::tuple<std::vector<Manga_t *>, bool> parseLatestEntries(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::tuple<std::vector<Manga_t *>, bool> parseLatestEntries(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual Manga_t *parseLatestEntry(Element &element)
  {
    ErrNotImplemented;
  }

  virtual std::string searchMangaSelector()
  {
    return NULL;
  }

  virtual std::string searchMangaNextSelector()
  {
    return NULL;
  }

  virtual std::string searchMangaRequest(int page, const std::string &query, const std::vector<FilterKV> &filters)
  {
    return NULL;
  }

  virtual std::tuple<std::vector<Manga_t *>, bool> parseSearchEntries(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::tuple<std::vector<Manga_t *>, bool> parseSearchEntries(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual Manga_t *parseSearchEntry(Element &element)
  {
    ErrNotImplemented;
  }

  virtual Manga_t *parseManga(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual Manga_t *parseManga(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual std::string chaptersSelector()
  {
    return NULL;
  }

  virtual std::string chaptersRequest(const Manga_t &manga)
  {
    return NULL;
  }

  virtual std::vector<Chapter_t *> parseChapterEntries(const Manga_t &manga, const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::vector<Chapter_t *> parseChapterEntries(const Manga_t &manga, HTML &html)
  {
    ErrNotImplemented;
  }

  virtual Chapter_t *parseChapterEntry(const Manga_t &manga, Element &element)
  {
    ErrNotImplemented;
  }

  virtual std::string pagesRequest(const std::string &path)
  {
    return NULL;
  }

  virtual std::vector<std::string> parsePages(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::vector<std::string> parsePages(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual const std::vector<Filter> &getFilters()
  {
    ErrNotImplemented;
  }

protected:
  std::string prependBaseUrl(const std::string &path);

private:
  std::tuple<std::vector<Manga_t *>, bool> getLatests(int page = 1);
  std::tuple<std::vector<Manga_t *>, bool> searchManga(int page = 1,
                                                       const std::string &query = "",
                                                       const std::vector<FilterKV> &filters = {});
  Manga_t *getManga(const std::string &path);
  std::vector<Chapter_t *> getChapters(const Manga_t &manga);
  std::vector<Chapter_t *> getChapters(const std::string &path);
  std::vector<std::string> getPages(const std::string &path);

  const std::map<std::string, Filter> &getFiltersMap();
};

#endif  // NONBIRI_CORE_EXTENSION_H_