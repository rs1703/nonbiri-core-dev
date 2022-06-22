#include <core/http/client.h>
#include <core/http/interceptor.h>

namespace Http
{
Interceptor::Chain::Chain(const Client &client, Request &request) : client(client), request(request) {}

std::shared_ptr<Response> Interceptor::Chain::proceed()
{
  if (done)
    return mResponse;

  done = true;
  mResponse = client.send(request);
  return mResponse;
}
}  // namespace Http