#ifndef NONBIRI_CORE_MODELS_H_
#define NONBIRI_CORE_MODELS_H_

#include <memory>
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

#define MangaPtr std::shared_ptr<Manga>

struct Chapter
{
  // e.g., /chapter/path
  std::string path;

  std::string name;
  int publishedAt;
  std::vector<std::string> groups;
};

#define ChapterPtr std::shared_ptr<Chapter>

#endif  // NONBIRI_CORE_MODELS_H_