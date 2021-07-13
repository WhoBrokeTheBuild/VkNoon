#include <Noon/Noon.hpp>

namespace noon {

NOON_API
Version GetVersion()
{
    return Version(NOON_VERSION_MAJOR, NOON_VERSION_MINOR, NOON_VERSION_PATCH);
}

} // namespace Noon
