#include <Noon/Noon.hpp>

#include <cstdio>

int main(int argc, char ** argv)
{
    printf("Noon Version: %s\n", Noon::GetVersionString().c_str());
    return 0;
}