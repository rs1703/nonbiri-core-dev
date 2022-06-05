#include <core/core.h>

namespace Http
{
init_t init {nullptr};
setOpt_t setOpt {nullptr};
perform_t perform {nullptr};
cleanup_t cleanup {nullptr};
getInfo_t getInfo {nullptr};
slist_append_t slist_append {nullptr};
slist_freeAll_t slist_freeAll {nullptr};
getError_t getError {nullptr};
};  // namespace Http

namespace Core
{
void initialize(Http::init_t initPtr,
                Http::setOpt_t setOptPtr,
                Http::perform_t performPtr,
                Http::cleanup_t cleanupPtr,
                Http::getInfo_t getInfoPtr,
                Http::slist_append_t slist_appendPtr,
                Http::slist_freeAll_t slist_freeAllPtr,
                Http::getError_t getErrorPtr)
{
  Http::init = initPtr;
  Http::setOpt = setOptPtr;
  Http::perform = performPtr;
  Http::cleanup = cleanupPtr;
  Http::getInfo = getInfoPtr;
  Http::slist_append = slist_appendPtr;
  Http::slist_freeAll = slist_freeAllPtr;
  Http::getError = getErrorPtr;
}
};  // namespace Core