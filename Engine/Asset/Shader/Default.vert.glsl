#version 450 core

#include <Transform.inc.glsl>
#include <VertexAttributes.inc.glsl>

layout(location = 0) out vec4 v_Color;

void main() {
    gl_Position = u_MVP * a_Position;
    v_Color = a_Normal;
}
