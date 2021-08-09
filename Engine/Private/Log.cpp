#include <Noon/Log.hpp>

namespace noon {

NOON_API
void LogMessage(StringView tag, StringView message)
{
    fmt::print("({}) {}\n", tag, message);
}

} // namespace noon