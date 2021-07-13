#ifndef NOON_STRING_HPP
#define NOON_STRING_HPP

#include <Noon/Config.hpp>

#include <string>
#include <string_view>

namespace noon {

using std::string;
using std::string_view;

inline bool StringEqualCaseInsensitive(string_view a, string_view b)
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
    std::wstring ConvertUTF8ToWideString(string str);

    NOON_API
    string ConvertWideStringToUTF8(std::wstring str);

#endif // defined(NOON_PLATFORM_WINDOWS)


} // namespace noon

#endif // NOON_STRING_HPP