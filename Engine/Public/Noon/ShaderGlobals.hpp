#ifndef NOON_SHADER_GLOBALS_HPP
#define NOON_SHADER_GLOBALS_HPP

#include <Noon/Config.hpp>
#include <Noon/Math.hpp>

namespace noon {

struct ShaderGlobals
{
public:

    static const unsigned Binding = 0;

    alignas(8) Vec2 Resolution;

    alignas(8) Vec2 Mouse;

    alignas(4) unsigned FrameCount;

    alignas(4) float FrameSpeedRatio;

}; // struct ShaderGlobals

} // namespace noon

#endif // NOON_SHADER_GLOBALS_HPP