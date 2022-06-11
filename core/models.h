#ifndef NONBIRI_CORE_MODELS_H_
#define NONBIRI_CORE_MODELS_H_

#include <string>
#include <vector>

enum class MangaStatus
{
  None,
  Ongoing,
  Completed,
  Hiatus,
  Cancelled,
};

struct Manga_t
{
  std::string path {};
  std::string coverUrl {};
  std::string title {};
  std::string description {};

  MangaStatus status {None};

  std::vector<std::string> artists {};
  std::vector<std::string> authors {};
  std::vector<std::string> genres {};
};

struct Chapter_t
{
  std::string path {};
  std::string name {};
  int64_t publishedAt {};

  std::vector<std::string> groups {};
};

#endif  // NONBIRI_CORE_MODELS_H_