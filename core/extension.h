#ifndef NONBIRI_CORE_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_H_

#include <atomic>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

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
  std::string id;
  std::string baseUrl;
  std::string name;
  std::string language;
  std::string version;

  bool isNsfw;
};

struct Filter
{
  std::string key;
  std::string value;
};

struct FilterInfo
{
  std::string name;
  std::string key;
  std::map<std::string, std::string> values;
};

class Manager;
class Api;
class Web;

class Extension : public ExtensionInfo
{
  friend class Manager;
  friend class Api;
  friend class Web;

  std::map<std::string, FilterInfo> filtersMap;
  std::atomic_bool hasUpdate = false;

public:
  bool useApi;

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

  virtual std::tuple<std::vector<Manga *>, bool> parseLatestEntries(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::tuple<std::vector<Manga *>, bool> parseLatestEntries(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual Manga *parseLatestEntry(Element &element)
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

  virtual std::string searchMangaRequest(int page, const std::string &query, const std::vector<Filter> &filters)
  {
    return NULL;
  }

  virtual std::tuple<std::vector<Manga *>, bool> parseSearchEntries(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::tuple<std::vector<Manga *>, bool> parseSearchEntries(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual Manga *parseSearchEntry(Element &element)
  {
    ErrNotImplemented;
  }

  virtual Manga *parseManga(const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual Manga *parseManga(HTML &html)
  {
    ErrNotImplemented;
  }

  virtual std::string chaptersSelector()
  {
    return NULL;
  }

  virtual std::string chaptersRequest(const Manga &manga)
  {
    return NULL;
  }

  virtual std::vector<Chapter *> parseChapterEntries(const Manga &manga, const std::string &response)
  {
    ErrNotImplemented;
  }

  virtual std::vector<Chapter *> parseChapterEntries(const Manga &manga, HTML &html)
  {
    ErrNotImplemented;
  }

  virtual Chapter *parseChapterEntry(const Manga &manga, Element &element)
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

  virtual const std::vector<FilterInfo> &getFilters()
  {
    ErrNotImplemented;
  }

protected:
  std::string prependBaseUrl(const std::string &path);

private:
  std::tuple<std::vector<MangaPtr>, bool> getLatests(int page = 1);
  std::tuple<std::vector<MangaPtr>, bool> searchManga(int page = 1,
                                                      const std::string &query = "",
                                                      const std::vector<Filter> &filters = {});
  MangaPtr getManga(const std::string &path);
  std::vector<ChapterPtr> getChapters(Manga &manga);
  std::vector<ChapterPtr> getChapters(const std::string &path);
  std::vector<std::string> getPages(const std::string &path);

  const std::map<std::string, FilterInfo> &getFiltersMap();
  std::tuple<std::vector<MangaPtr>, bool> makeMangaEntries(const std::tuple<std::vector<Manga *>, bool> &result);
  std::vector<ChapterPtr> makeChapterEntries(const std::vector<Chapter *> &result);
};

#endif  // NONBIRI_CORE_EXTENSION_H_