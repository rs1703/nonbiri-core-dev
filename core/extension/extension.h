#ifndef NONBIRI_CORE_EXTENSION_EXTENSION_H_
#define NONBIRI_CORE_EXTENSION_EXTENSION_H_

#include <memory>
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
  const std::shared_ptr<extension> dummyCreate() \
  { \
    return std::make_shared<extension>(); \
  } \
  EXPORT_API const std::shared_ptr<extension> create() \
  { \
    return dummyCreate(); \
  }

struct ExtensionInfo
{
  std::string id;
  std::string baseUrl;
  std::string name;
  std::string language;
  std::string version;
};

class Extension : public ExtensionInfo
{
public:
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
  virtual std::string searchMangaRequest(int page, const std::string &query);
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

  virtual std::string pagesRequest(const std::string &path);
  virtual std::vector<std::string> parsePages(const std::string &response);
  virtual std::vector<std::string> parsePages(CHtml &html);

protected:
  std::string prependBaseUrl(const std::string &path);
};

#endif  // NONBIRI_CORE_EXTENSION_EXTENSION_H_