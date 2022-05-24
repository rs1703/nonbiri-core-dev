#ifndef NONBIRI_CORE_MODELS_H_
#define NONBIRI_CORE_MODELS_H_

#include <memory>
#include <string>
#include <vector>

enum class MangaStatus
{
  Ongoing = 1,
  Completed,
  Hiatus,
  Dropped,
};

struct Manga_t
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

#define MangaPtr_t std::shared_ptr<Manga_t>

struct Chapter_t
{
  // e.g., /chapter/path
  std::string path;

  std::string name;
  int64_t publishedAt;
  std::vector<std::string> groups;
};

#define ChapterPtr_t std::shared_ptr<Chapter_t>

#endif  // NONBIRI_CORE_MODELS_H_