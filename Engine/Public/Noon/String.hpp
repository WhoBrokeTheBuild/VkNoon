#ifndef NOON_STRING_HPP
#define NOON_STRING_HPP

#include <Noon/Config.hpp>

#include <string>
#include <string_view>

namespace noon {

using String = std::string;
using StringView = std::string_view;

inline bool StringEqualCaseInsensitive(StringView a, StringView b)
{
    // TODO: UTF-8
    return std::equal(
        a.begin(), a.end(),
        b.begin(), b.end(),
        [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        }
    );
}

#if defined(NOON_PLATFORM_WINDOWS)

    NOON_API
    std::wstring ConvertUTF8ToWideString(String str);

    NOON_API
    String ConvertWideStringToUTF8(std::wstring str);

#endif // defined(NOON_PLATFORM_WINDOWS)


} // namespace noon

#endif // NOON_STRING_HPP