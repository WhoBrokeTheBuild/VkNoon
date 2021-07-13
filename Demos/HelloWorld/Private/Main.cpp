#include <Noon/Noon.hpp>
#include <Noon/Application.hpp>

#include <cstdio>

using namespace noon;

int main(int argc, char ** argv)
{
    try {
        Application app;
        app.Run();
    }
    catch (std::exception& e) {
        printf("Exception: %s\n", e.what());
    }

    return 0;
}