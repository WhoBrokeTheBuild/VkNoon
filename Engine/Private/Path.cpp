#include <Noon/Path.hpp>

#include <algorithm>
#include <cstdio>
#include <climits>
#include <sstream>

#if defined(NOON_PLATFORM_WINDOWS)

    #include <direct.h>

#else

    #include <unistd.h>

#endif

namespace noon {

NOON_API
Path::Path(const Path& rhs)
    : _path(rhs._path)
{ }

NOON_API
Path::Path(const string& str)
    : _path(str)
{
    Normalize();
}

NOON_API
Path::Path(const char * cstr)
    : _path(cstr)
{
    Normalize();
}

NOON_API
Path& Path::Append(const Path& rhs)
{
    // If path is empty, and doesn't have a trailing slash, append one
    if (rhs._path.empty()) {
        if (!_path.empty() && _path.back() != Slash) {
            _path += Slash;
        }
        return *this;
    }

    if (IsAbsolute() && rhs.IsAbsolute() && GetRootPath() != rhs.GetRootPath()) {
        // Unable to append absolute paths
        _path = rhs._path;
        return *this;
    }


    if (_path.back() != Slash) {
        _path += Slash;
    }

    _path += rhs._path;
    return *this;
}

Path& Path::Concatenate(const Path& path)
{
    _path += path._path;
    return *this;
}

bool Path::Equals(const Path& rhs) const
{
    #if defined(NOON_PLATFORM_WINDOWS)

        return StringEqualCaseInsensitive(_path, rhs._path);

    #else

        return (_path == rhs._path);

    #endif
}

NOON_API
void Path::Normalize()
{
    if (_path.empty()) {
        return;
    }

    // TODO: Check valid UTF-8

    // TODO: Strip windows long filename marker "\\?\"

    #if defined(NOON_PLATFORM_WINDOWS)

        // Convert slashes to native format
        for (auto& c : _path) {
            if (c == '/') {
                c = '\\';
            }
        }

    #endif

    auto begin = _path.begin();
    auto end = _path.end();

    // Skip double slashes for paths starting like "\\server"
    if (_path.length() >= 2 && _path[0] == Slash && _path[1] == Slash) {
        begin += 2;
    }

    auto newEnd = std::unique(begin, end,
        [](char lhs, char rhs) {
            return (lhs == rhs && lhs == Slash);
        }
    );

    _path.erase(newEnd, end);
}

NOON_API
size_t Path::GetRootNameLength() const
{
    #if defined(NOON_PLATFORM_WINDOWS)

        // Check for windows drive letter, such as "C:"
        if (_path.length() >= 2 && _path[1] == ':') {
            char first = std::toupper(_path[0]);
            if (first >= 'A' && first <= 'Z') {
                return 2;
            }
        }

    #endif

    // Check for network path, such as "//server"
    if (_path.length() >= 3 && _path[0] == Slash && _path[1] == Slash) {
        if (_path[2] != Slash && std::isprint(_path[2])) {
            // Find first separator after server name
            size_t pos = _path.find_first_of(Slash, 3);
            if (pos == string::npos) {
                // The entire path is just a network share name
                return _path.length();
            }
            else {
                return pos;
            }
        }
    }

    // There is no root name
    return 0;
}

NOON_API
Path GetCurrentPath()
{
#if defined(NOON_PLATFORM_WINDOWS)

    std::unique_ptr<char> cwd(_getcwd(nullptr, 0));
    if (cwd) {
        return Path(string(cwd.get()));
    }

#else

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd))) {
        return Path(cwd);
    }

#endif
    
    return Path();
}

} // namespace noon