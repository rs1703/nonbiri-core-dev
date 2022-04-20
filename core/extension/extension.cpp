#include <core/extension/extension.h>

using std::string;
using std::tuple;
using std::vector;

Extension::Extension()
{
#ifdef ID
  id = ID;
#endif

#ifdef BASE_URL
  baseUrl = BASE_URL;
#endif

  if (baseUrl.back() == '/')
    baseUrl.pop_back();

#ifdef NAME
  name = NAME;
#endif

#ifdef LANGUAGE
  language = LANGUAGE;
#endif

#ifdef VERSION
  version = VERSION;
#endif

#if USE_API == 1
  useApi = true;
#endif
}

Extension::~Extension()
{
  // std::cout << "Extension::~Extension()" << std::endl;
}

std::string Extension::latestsSelector()
{
  return NULL;
};

std::string Extension::latestsNextSelector()
{
  return NULL;
};

std::string Extension::latestsRequest(int)
{
  return NULL;
};

tuple<vector<Manga *>, bool> Extension::parseLatestEntries(const string &)
{
  ErrNotImplemented;
}

tuple<vector<Manga *>, bool> Extension::parseLatestEntries(CHtml &)
{
  ErrNotImplemented;
}

Manga *Extension::parseLatestEntry(CElement &)
{
  ErrNotImplemented;
}

std::string Extension::searchMangaSelector()
{
  return NULL;
};

std::string Extension::searchMangaNextSelector()
{
  return NULL;
};

std::string Extension::searchMangaRequest(int, const std::string &)
{
  return NULL;
};

tuple<vector<Manga *>, bool> Extension::parseSearchEntries(const string &)
{
  ErrNotImplemented;
}

tuple<vector<Manga *>, bool> Extension::parseSearchEntries(CHtml &)
{
  ErrNotImplemented;
}

Manga *Extension::parseSearchEntry(CElement &)
{
  ErrNotImplemented;
}

Manga *Extension::parseManga(const string &)
{
  ErrNotImplemented;
}

Manga *Extension::parseManga(CHtml &)
{
  ErrNotImplemented;
}

std::string Extension::chaptersSelector()
{
  return NULL;
};

std::string Extension::chaptersRequest(const Manga &)
{
  return NULL;
};

vector<Chapter *> Extension::parseChapterEntries(const Manga &, const string &)
{
  ErrNotImplemented;
}

vector<Chapter *> Extension::parseChapterEntries(const Manga &, CHtml &)
{
  ErrNotImplemented;
}

Chapter *Extension::parseChapterEntry(const Manga &, CElement &)
{
  ErrNotImplemented;
}

std::string Extension::pagesRequest(const std::string &)
{
  return NULL;
}

vector<string> Extension::parsePages(const string &)
{
  ErrNotImplemented;
}

vector<string> Extension::parsePages(CHtml &)
{
  ErrNotImplemented;
}

std::string Extension::prependBaseUrl(const std::string &path)
{
  if (path.find(baseUrl) == 0 || path.find("http") == 0) {
    return path;
  }

  std::string result = baseUrl;
  if (path.front() != '/')
    result += '/';
  result += path;

  return result;
}