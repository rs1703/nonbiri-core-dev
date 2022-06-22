#ifndef NONBIRI_CORE_HTTP_INTERCEPTOR_H_
#define NONBIRI_CORE_HTTP_INTERCEPTOR_H_

#include <memory>

#include <core/http/types.h>

namespace Http
{
class Client;
struct Interceptor
{
  class Chain
  {
    std::shared_ptr<Response> mResponse;
    bool done {};

  public:
    const Client &client;
    Request &request;

    Chain(const Client &client, Request &request);
    std::shared_ptr<Response> proceed();
  };

  virtual std::shared_ptr<Response> intercept(Chain &chain) = 0;
};
}  // namespace Http

#endif  // NONBIRI_CORE_HTTP_INTERCEPTOR_H_