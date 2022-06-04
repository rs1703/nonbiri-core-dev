#include <core/core.h>

namespace Http
{
init_t init {nullptr};
setOpt_t setOpt {nullptr};
perform_t perform {nullptr};
cleanup_t cleanup {nullptr};
getInfo_t getInfo {nullptr};
getError_t getError {nullptr};
};  // namespace Http

namespace Core
{
void initialize(Http::init_t initPtr,
                Http::setOpt_t setOptPtr,
                Http::perform_t performPtr,
                Http::cleanup_t cleanupPtr,
                Http::getInfo_t getInfoPtr,
                Http::getError_t getErrorPtr)
{
  Http::init = initPtr;
  Http::setOpt = setOptPtr;
  Http::perform = performPtr;
  Http::cleanup = cleanupPtr;
  Http::getInfo = getInfoPtr;
  Http::getError = getErrorPtr;
}
};  // namespace Core