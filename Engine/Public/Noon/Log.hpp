#ifndef NOON_LOG_HPP
#define NOON_LOG_HPP

#include <Noon/Config.hpp>
#include <Noon/Path.hpp>
#include <Noon/Version.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/os.h>
#include <fmt/color.h>

namespace noon {

#define NOON_ANCHOR (fmt::format("{}:{}", noon::Path(__FILE__).GetFilename().ToCString(), __LINE__))

NOON_API
void LogMessage(StringView tag, StringView message);

template <class... Args>
inline void Log(StringView tag, StringView format, const Args&... args) {
    LogMessage(tag, fmt::format(format, args...));
}

} // namespace noon

#endif // NOON_LOG_HPP