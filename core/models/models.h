#ifndef NONBIRI_CORE_MODELS_MODELS_H_
#define NONBIRI_CORE_MODELS_MODELS_H_

#include <string>
#include <vector>

enum MangaStatus
{
  Ongoing = 1,
  Completed,
  Hiatus,
  Dropped,
};

struct Manga
{
  std::string url;
  std::string coverUrl;

  std::string title;
  std::string description;
  MangaStatus status;

  std::vector<std::string> artists;
  std::vector<std::string> authors;
  std::vector<std::string> genres;
};

struct Chapter
{
  std::string url;
  std::string name;
  int publishedAt;
  std::vector<std::string> groups;
};

#endif  // NONBIRI_CORE_MODELS_MODELS_H_