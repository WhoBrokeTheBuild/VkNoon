#ifndef DUSK_GLOBALS_INC_GLSL
#define DUSK_GLOBALS_INC_GLSL

layout(binding = 0, std140) uniform DuskGlobals
{
    vec2 u_Resolution;
    vec2 u_Mouse;
    int u_FrameCount;
    float u_TotalTime;
    float u_FrameSpeedRatio;
    
};

#endif // DUSK_GLOBALS_INC_GLSL