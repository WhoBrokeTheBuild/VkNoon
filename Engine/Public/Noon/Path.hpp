#ifndef NOON_PATH_HPP
#define NOON_PATH_HPP

#include <Noon/Config.hpp>
#include <Noon/String.hpp>

#include <fmt/format.h>

namespace noon {

class NOON_API Path
{
public:

    static const char Slash = NOON_PATH_SLASH;

    static const char Separator = NOON_PATH_SEPARATOR;

    Path() = default;

    Path(const Path& rhs);

    Path(const String& str);

    Path(const char * cstr);

    // Appends a new element to the path with a separator
    Path& Append(const Path& rhs);

    // Concatenates the paths without adding a separator
    Path& Concatenate(const Path& rhs);

    inline String ToString() const {
        return _path;
    }

    inline const char * ToCString() const {
        return _path.c_str();
    }

    inline operator String() const {
        return _path;
    }

    inline bool IsEmpty() const {
        return _path.empty();
    }

    inline bool IsAbsolute() const {
        return HasRootDirectory();
    }

    inline bool IsRelative() const {
        return !HasRootDirectory();
    }

    inline bool HasRootName() const {
        return (GetRootNameLength() > 0);
    }

    /// The root identifier on a filesystem with multiple roots, such as "C:" or "//server"
    inline Path GetRootName() const {
        return _path.substr(0, GetRootNameLength());
    }

    inline bool HasRootDirectory() const {
        size_t rootNameLen = GetRootNameLength();
        return (_path.length() > rootNameLen && _path[rootNameLen] == Slash);
    }

    inline Path GetRootDirectory() const {
        if (HasRootDirectory()) {
            return String(1, Slash);
        }
        return Path();
    }

    inline bool HasRootPath() const {
        return (HasRootName() || HasRootDirectory());
    }

    inline Path GetRootPath() const {
        return GetRootName() + GetRootDirectory();
    }

    inline Path GetParentPath() const {
        size_t pivot = _path.find_last_of(Slash);
        return (
            pivot <= GetRootNameLength() || pivot == String::npos
            ? _path
            : _path.substr(0, pivot)
        );
    }

    inline Path GetFilename() const {
        size_t pivot = _path.find_last_of(Slash);
        return (
            pivot <= GetRootNameLength() || pivot == String::npos
            ? _path
            : _path.substr(pivot + 1)
        );
    }

    inline Path GetStem() const {
        String filename = GetFilename();
        size_t pivot = filename.find_last_of('.');
        return (
            pivot == 0 || pivot == String::npos
            ? filename
            : filename.substr(0, pivot)
        );
    }

    inline Path GetExtension() const {
        String filename = GetFilename();
        size_t pivot = filename.find_last_of('.');
        return (
            pivot == 0 || pivot == String::npos
            ? String()
            : filename.substr(pivot + 1)
        );
    }

    inline Path& operator=(const Path& rhs) {
        _path = rhs._path;
        return *this;
    }

    inline Path& operator/=(const Path& rhs) {
        return Append(rhs);
    }

    inline Path& operator+=(const Path& rhs) {
        return Concatenate(rhs);
    }

    bool Equals(const Path& rhs) const;

    inline friend bool operator==(const Path& lhs, const Path& rhs) {
        return lhs.Equals(rhs);
    }

    inline friend bool operator!=(const Path& lhs, const Path& rhs) {
        return !lhs.Equals(rhs);
    }

    inline friend Path operator/(const Path& lhs, const Path& rhs)
    {
        Path tmp(lhs);
        tmp /= rhs;
        return tmp;
    }

    inline friend Path operator+(const Path& lhs, const Path& rhs)
    {
        Path tmp(lhs);
        tmp += rhs;
        return tmp;
    }

private:

    void Normalize();

    size_t GetRootNameLength() const;

    String _path;

}; // class Path

NOON_API
Path GetCurrentPath();

} // namespace noon

template<>
struct fmt::formatter<noon::Path> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const noon::Path& path, FormatContext& ctx) {
        return formatter<std::string_view>::format(std::string_view(path.ToString()), ctx);
    }
};

#endif // NOON_PATH_HPP