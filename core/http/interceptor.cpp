#include <core/http/client.h>
#include <core/http/interceptor.h>

namespace Http
{
Interceptor::Chain::Chain(const Client &client, Request &request) : mClient(client), mRequest(request) {}

Request &Interceptor::Chain::request() const
{
  return mRequest;
}

std::shared_ptr<Response> Interceptor::Chain::proceed()
{
  if (done)
    return mResponse;

  done = true;
  mResponse = mClient.send(mRequest);
  return mResponse;
}
};  // namespace Http