#include "HelloWorldApplication.hpp"

HelloWorldApplication::HelloWorldApplication()
{
    GetGraphicsDriver()->SetWindowSize({ 1024, 768 });
}

Version HelloWorldApplication::GetVersion()
{
    return Version(1, 0, 0);
}

String HelloWorldApplication::GetName()
{
    return "HelloWorld";
}