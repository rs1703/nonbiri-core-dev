#include <iostream>
#include <thread>

#include <core/core.h>
#include <core/http/ratelimiter.h>

namespace Http
{
RateLimiter::RateLimiter(int64_t size, int64_t period) : tokens {size}, size {size}, period {period} {}

void RateLimiter::acquire()
{
  std::lock_guard lock {mutex};

  auto seconds = std::chrono::seconds(period);
  auto now     = std::chrono::system_clock::now();
  auto next    = lastAcquired + seconds;

  if (now > next) {
    tokens = size;
  } else if (tokens <= 0) {
    std::this_thread::sleep_until(next);
    tokens = size;
  }

  lastAcquired = now;
  --tokens;
}
}  // namespace Http