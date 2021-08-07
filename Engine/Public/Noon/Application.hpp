#ifndef NOON_APPLICATION_HPP
#define NOON_APPLICATION_HPP

#include <Noon/Config.hpp>
#include <Noon/GraphicsDriver.hpp>
#include <Noon/Version.hpp>

namespace noon {

class NOON_API Application
{
public:

    NOON_DISALLOW_COPY_AND_ASSIGN(Application);

    static Application * GetInstance() {
        return _Instance;
    }

    Application();

    virtual ~Application();

    virtual Version GetVersion();

    virtual string GetName();

    float GetTargetFPS() const {
        return _targetFPS;
    }

    void SetTargetFPS(float fps) {
        _targetFPS = fps;
    }

    GraphicsDriver * GetGraphicsDriver() const {
        return _graphicsDriver;
    }

    void Run();

    void Stop();

private:

    static Application * _Instance;

    float _targetFPS = 60.0f;

    bool _running = false;

    GraphicsDriver * _graphicsDriver = nullptr;

}; // class Application

} // namespace noon

#endif // NOON_APPLICATION_HPP