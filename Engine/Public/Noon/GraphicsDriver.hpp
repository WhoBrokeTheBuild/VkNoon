#ifndef NOON_GRAPHICS_DRIVER_HPP
#define NOON_GRAPHICS_DRIVER_HPP

#include <Noon/Config.hpp>

#include <SDL.h>

namespace noon {

class NOON_API GraphicsDriver
{
public:

    GraphicsDriver();

    ~GraphicsDriver();

    void ProcessEvents();
    
    void Render();

private:

    static GraphicsDriver * _Instance;

    SDL_Window * _sdlWindow;


}; // class GraphicsDriver

} // namespace noon

#endif // NOON_GRAPHICS_DRIVER_HPP