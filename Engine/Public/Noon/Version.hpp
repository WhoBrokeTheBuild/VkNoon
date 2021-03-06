#ifndef NOON_VERSION_HPP
#define NOON_VERSION_HPP

#include <Noon/Config.hpp>
#include <Noon/String.hpp>

#include <fmt/format.h>

#include <regex>
#include <tuple>

namespace noon {

struct NOON_API Version
{
    int Major;
    int Minor;
    int Patch;

    Version(int major = 0, int minor = 0, int patch = 0)
        : Major(major)
        , Minor(minor)
        , Patch(patch)
    { }

    Version(const String& str)
    {
        FromString(str);
    }

    Version(const Version& rhs)
        : Major(rhs.Major)
        , Minor(rhs.Minor)
        , Patch(rhs.Patch)
    { }

    inline Version& operator=(const Version& rhs)
    {
        Major = rhs.Major;
        Minor = rhs.Minor;
        Patch = rhs.Patch;
        return *this;
    }

    inline friend bool operator==(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == 0);
    }

    inline friend bool operator!=(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) != 0);
    }

    inline friend bool operator>(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == 1);
    }

    inline friend bool operator>=(const Version& lhs, const Version& rhs)
    {
        int res = Version::Compare(lhs, rhs);
        return (res == 0 || res == 1);
    }

    inline friend bool operator<(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == -1);
    }

    inline friend bool operator<=(const Version& lhs, const Version& rhs)
    {
        int res = Version::Compare(lhs, rhs);
        return (res == 0 || res == -1);
    }

    inline String ToString() const
    {
        return fmt::format("{}.{}.{}", 
            Major, Minor, Patch);
    }

    inline operator String() const
    {
        return ToString();
    }

    inline void FromString(const String& string)
    {
        std::smatch match;
        std::regex regex("^.*?([0-9])+\\.([0-9]+)\\.([0-9]+).*");
        std::regex_match(string, match, regex);

        if (match.size() == 4) {
            Major = std::strtol(match[1].str().c_str(), nullptr, 10);
            Minor = std::strtol(match[2].str().c_str(), nullptr, 10);
            Patch = std::strtol(match[3].str().c_str(), nullptr, 10);
        }
    }

    inline static int Compare(const Version& a, const Version& b)
    {
        auto cmp = [](int a, int b)
        {
            if (a == b) {
                return 0;
            }
            if (a > b) {
                return 1;
            }
            return -1;
        };

        int result = cmp(a.Major, b.Major);
        if (result == 0) {
            result = cmp(a.Minor, b.Minor);
            if (result == 0) {
                result = cmp(a.Patch, b.Patch);
            }
        }
        return result;
    }

}; // struct Version

} // namespace noon

template<>
struct fmt::formatter<noon::Version> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const noon::Version& version, FormatContext& ctx) {
        return formatter<std::string_view>::format(std::string_view(version.ToString()), ctx);
    }
};

#endif // NOON_VERSION_HPP