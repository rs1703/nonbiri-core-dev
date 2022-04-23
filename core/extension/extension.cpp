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

#ifdef IS_NSFW
  isNsfw = true;
#endif

#ifdef USE_API
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

tuple<vector<Manga *>, bool> Extension::parseLatestEntries(HTML &)
{
  ErrNotImplemented;
}

Manga *Extension::parseLatestEntry(Element &)
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

std::string Extension::searchMangaRequest(int, const std::string &, const std::vector<Filter> &)
{
  return NULL;
};

tuple<vector<Manga *>, bool> Extension::parseSearchEntries(const string &)
{
  ErrNotImplemented;
}

tuple<vector<Manga *>, bool> Extension::parseSearchEntries(HTML &)
{
  ErrNotImplemented;
}

Manga *Extension::parseSearchEntry(Element &)
{
  ErrNotImplemented;
}

Manga *Extension::parseManga(const string &)
{
  ErrNotImplemented;
}

Manga *Extension::parseManga(HTML &)
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

vector<Chapter *> Extension::parseChapterEntries(const Manga &, HTML &)
{
  ErrNotImplemented;
}

Chapter *Extension::parseChapterEntry(const Manga &, Element &)
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

vector<string> Extension::parsePages(HTML &)
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

const std::vector<FilterInfo> &Extension::getFilters()
{
  ErrNotImplemented;
}