#ifndef DUSK_TRANSFORM_INC_GLSL
#define DUSK_TRANSFORM_INC_GLSL

layout(binding = 1, std140) uniform DuskTransform
{
    mat4 u_Model;
    mat4 u_View;
    mat4 u_Proj;
    mat4 u_MVP;

};

#endif // DUSK_TRANSFORM_INC_GLSL