#include <Noon/Log.hpp>

namespace noon {

NOON_API
void LogMessage(string_view tag, string_view message)
{
    fmt::print("({}) {}\n", tag, message);
}

} // namespace noon