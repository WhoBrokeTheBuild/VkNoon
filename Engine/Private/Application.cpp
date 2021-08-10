#include <Noon/Application.hpp>
#include <Noon/Log.hpp>

#include <chrono>
#include <thread>

namespace noon {

Application * Application::_Instance = nullptr;

NOON_API
Application::Application()
{
    _Instance = this;
}

NOON_API
Application::~Application()
{
    _Instance = nullptr;
}

NOON_API
void Application::Init()
{
    _graphicsDriver = new GraphicsDriver;
}

NOON_API
void Application::Term()
{
    delete _graphicsDriver;
}

NOON_API
Version Application::GetVersion()
{
    return Version(0, 0, 0);
}

NOON_API
String Application::GetName()
{
    return "Noon";
}

NOON_API
void Application::Run()
{
    using namespace std::chrono;

    Init();

    auto startTime = high_resolution_clock::now();
    auto previousTime = startTime;
    
    _running = true;
    while (_running) {
        high_resolution_clock::time_point currentTime = high_resolution_clock::now();
        auto totalDuration = duration_cast<milliseconds>(currentTime - startTime);
        auto previousFrameDuration = duration_cast<microseconds>(currentTime - previousTime);
        previousTime = currentTime;

        // ctx->SetTotalDuration(totalDuration);
        // ctx->SetPreviousFrameDuration(previousFrameDuration);
        auto expectedFrameDuration = microseconds((int64_t)(1000000.0f / _targetFPS));
        auto frameEndTime = currentTime + expectedFrameDuration;


        _graphicsDriver->ProcessEvents();

        _graphicsDriver->Render();


        currentTime = high_resolution_clock::now();
        auto timeToSleep = duration_cast<milliseconds>(frameEndTime - currentTime);
        if (timeToSleep > 1ms) { // TODO: Find "minimum" sleep time
            std::this_thread::sleep_for(timeToSleep);
        }
    }

    Term();
}

void Application::Stop()
{
    _running = false;
}

} // namespace noon