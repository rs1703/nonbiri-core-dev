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
  // e.g., /manga/path
  std::string path;

  // Full path (e.g., domain.com/covers/path)
  std::string coverUrl;

  std::string title;
  std::string description;

  // Publication status
  MangaStatus status;

  std::vector<std::string> artists;
  std::vector<std::string> authors;
  std::vector<std::string> genres;
};

struct Chapter
{
  // e.g., /chapter/path
  std::string path;

  std::string name;
  int publishedAt;
  std::vector<std::string> groups;
};

#endif  // NONBIRI_CORE_MODELS_MODELS_H_