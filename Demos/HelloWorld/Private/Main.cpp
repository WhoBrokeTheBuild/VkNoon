#include <cstdio>

#include <VkNoon/VkNoon.hpp>

int main(int argc, char ** argv)
{
    printf("VkNoon Version: %s\n", VkNoon::GetVersionString().c_str());
    return 0;
}