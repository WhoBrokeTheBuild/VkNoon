#include <Noon/GraphicsDriver.hpp>
#include <Noon/Application.hpp>
#include <Noon/Exception.hpp>
#include <Noon/Log.hpp>

namespace noon {

NOON_API
GraphicsDriver::GraphicsDriver()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw Exception("SDL_Init failed, {}", SDL_GetError());
    }

    SDL_version version;
    SDL_GetVersion(&version);
    Log(NOON_ANCHOR, "SDL Version: {}.{}.{}", 
        (int)version.major, (int)version.minor, (int)version.patch);

    _sdlWindow = SDL_CreateWindow(
        "Noon",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    if (!_sdlWindow) {
        throw Exception("SDL_CreateWindow failed, {}", SDL_GetError());
    }
}

NOON_API
GraphicsDriver::~GraphicsDriver()
{
    if (_sdlWindow) {
        SDL_DestroyWindow(_sdlWindow);
    }
    
    SDL_Quit();
}

void GraphicsDriver::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Application::GetInstance()->Stop();
        }
    }
}

void GraphicsDriver::Render()
{

}

} // namespace noon