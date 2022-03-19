#ifndef NONBIRI_CORE_EXTENSION_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_EXTENSION_H_

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <core/models/models.h>
#include <core/parser/parser.h>

#define ErrNotImplemented throw std::runtime_error("Not implemented")

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define EXPORT_API extern "C" __declspec(dllexport)
#else
#  define EXPORT_API extern "C"
#endif

#define RegisterExtension(extension) \
  EXPORT_API Extension *create() \
  { \
    return new extension(); \
  }

class Extension
{
public:
  std::string baseUrl;
  std::string name;
  std::string language;
  std::string version;
  bool useApi;

public:
  Extension();
  ~Extension();

  virtual std::string latestsSelector();
  virtual std::string latestsNextSelector();
  virtual std::string latestsRequest(int page);
  virtual std::tuple<std::vector<Manga *>, bool> parseLatestEntries(const std::string &response);
  virtual std::tuple<std::vector<Manga *>, bool> parseLatestEntries(CHtml &html);
  virtual Manga *parseLatestEntry(CElement &element);

  virtual std::string searchMangaSelector();
  virtual std::string searchMangaNextSelector();
  virtual std::string searchMangaRequest(int page, const char *query);
  virtual std::tuple<std::vector<Manga *>, bool> parseSearchEntries(const std::string &response);
  virtual std::tuple<std::vector<Manga *>, bool> parseSearchEntries(CHtml &html);
  virtual Manga *parseSearchEntry(CElement &element);

  virtual Manga *parseManga(const std::string &response);
  virtual Manga *parseManga(CHtml &html);

  virtual std::string chaptersSelector();
  virtual std::string chaptersRequest(const Manga &manga);
  virtual std::vector<Chapter *> parseChapterEntries(const Manga &manga, const std::string &response);
  virtual std::vector<Chapter *> parseChapterEntries(const Manga &manga, CHtml &html);
  virtual Chapter *parseChapterEntry(const Manga &manga, CElement &element);

  virtual std::string pagesRequest(const Chapter &chapter);
  virtual std::vector<std::string> parsePages(const Chapter &chapter, const std::string &response);
  virtual std::vector<std::string> parsePages(const Chapter &chapter, CHtml &html);
};

#endif  // NONBIRI_CORE_EXTENSION_EXTENSION_H_