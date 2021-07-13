#ifndef NOON_EXCEPTION_HPP
#define NOON_EXCEPTION_HPP

#include <Noon/Config.hpp>
#include <Noon/String.hpp>

#include <fmt/core.h>

#include <stdexcept>

namespace noon {

class NOON_API Exception : public std::exception
{
public:

    Exception(string_view message) noexcept
        : _message(message)
    { }

    template <class... Args>
    Exception(string_view format, const Args&... args) noexcept
        : _message(fmt::format(format, args...))
    { }

    const char * what() const noexcept override {
        return _message.c_str();
    }

private:

    string _message;

}; // class Exception

} // namespace noon

#endif // NOON_EXCEPTION_HPP