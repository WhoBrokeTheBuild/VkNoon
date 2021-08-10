#ifndef HELLO_WORLD_APPLICATION_HPP
#define HELLO_WORLD_APPLICATION_HPP

#include <Noon/Noon.hpp>
#include <Noon/Application.hpp>

using namespace noon;

class HelloWorldApplication : public noon::Application
{
public:

    HelloWorldApplication() = default;

    ~HelloWorldApplication() = default;

    Version GetVersion() override;

    String GetName() override;

    void Init() override;

};

#endif // HELLO_WORLD_APPLICATION_HPP