#include "HelloWorldApplication.hpp"

#include <cstdio>

int main(int argc, char ** argv)
{
    try {
        HelloWorldApplication app;
        app.Run();
    }
    catch (std::exception& e) {
        printf("Exception: %s\n", e.what());
    }

    return 0;
}