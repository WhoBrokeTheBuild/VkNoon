#include "HelloWorldApplication.hpp"

Version HelloWorldApplication::GetVersion()
{
    return Version(1, 0, 0);
}

String HelloWorldApplication::GetName()
{
    return "HelloWorld";
}

void HelloWorldApplication::Init()
{
    Application::Init();

    GetGraphicsDriver()->SetWindowSize({ 1024, 768 });
}
