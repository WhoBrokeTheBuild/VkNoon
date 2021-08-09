#ifndef NOON_SHADER_TRANSFORM_HPP
#define NOON_SHADER_TRANSFORM_HPP

#include <Noon/Config.hpp>
#include <Noon/Math.hpp>

namespace noon {

struct ShaderTransform
{
public:

    static const unsigned Binding = 1;

    alignas(64) Mat4 Model;

    alignas(64) Mat4 View;

    alignas(64) Mat4 Projection;

    alignas(64) Mat4 MVP;

}; // struct ShaderTransform

} // namespace noon

#endif // NOON_SHADER_TRANSFORM_HPP