#ifndef NONBIRI_CORE_HTTP_RATELIMITER_H_
#define NONBIRI_CORE_HTTP_RATELIMITER_H_

#include <chrono>
#include <mutex>

namespace Http
{
class RateLimiter
{
  int64_t tokens {};
  int64_t size {};
  int64_t period {};
  std::chrono::time_point<std::chrono::system_clock> lastAcquired {};
  std::mutex mutex {};

public:
  RateLimiter(int64_t size, int64_t period = 1);
  void acquire();
};
};  // namespace Http

#endif  // NONBIRI_CORE_HTTP_RATELIMITER_H_