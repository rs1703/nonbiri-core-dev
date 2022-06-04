#ifndef NONBIRI_CORE_CORE_H_
#define NONBIRI_CORE_CORE_H_

#include <core/utility.h>
#include <curl/curl.h>

namespace Http
{
typedef CURL *(*init_t)(void);
typedef CURLcode (*setOpt_t)(CURL *, CURLoption, ...);
typedef CURLcode (*perform_t)(CURL *handle);
typedef void (*cleanup_t)(CURL *handle);
typedef CURLcode (*getInfo_t)(CURL *handle, CURLINFO info, ...);
typedef const char *(*getError_t)(CURLcode code);

extern init_t init;
extern setOpt_t setOpt;
extern perform_t perform;
extern cleanup_t cleanup;
extern getInfo_t getInfo;
extern getError_t getError;
};  // namespace Http

namespace Core
{
typedef void (*initialize_t)(
  Http::init_t, Http::setOpt_t, Http::perform_t, Http::cleanup_t, Http::getInfo_t, Http::getError_t);

EXPORT_API void initialize(
  Http::init_t, Http::setOpt_t, Http::perform_t, Http::cleanup_t, Http::getInfo_t, Http::getError_t);
};  // namespace Core
#endif  // NONBIRI_CORE_CORE_H_