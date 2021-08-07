#include <Noon/GraphicsDriver.hpp>
#include <Noon/Application.hpp>
#include <Noon/Exception.hpp>
#include <Noon/Log.hpp>

namespace noon {

NOON_API
GraphicsDriver::GraphicsDriver()
{
    InitWindow();
    InitInstance();
    InitSurface();
    InitLogicalDevice();
    InitAllocator();
    InitSwapChain();
}

NOON_API
GraphicsDriver::~GraphicsDriver()
{
    TermSwapChain();
    TermAllocator();
    TermLogicalDevice();
    TermSurface();
    TermInstance();
    TermWindow();
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

bool GraphicsDriver::IsDeviceSuitable(const VkPhysicalDevice device)
{
    return true;
}

std::vector<const char *> GraphicsDriver::GetRequiredLayers()
{
    return {};
}

std::vector<const char *> GraphicsDriver::GetRequiredDeviceExtensions()
{
    return {};
}

std::vector<const char *> GraphicsDriver::GetRequiredInstanceExtensions()
{
    return {};
}

void GraphicsDriver::GetPhysicalDevice()
{

}

void GraphicsDriver::InitWindow()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw Exception("SDL_Init failed, {}", SDL_GetError());
    }

    SDL_version version;
    SDL_GetVersion(&version);
    Log(NOON_ANCHOR, "SDL {}.{}.{}", 
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

void GraphicsDriver::TermWindow()
{
    if (_sdlWindow) {
        SDL_DestroyWindow(_sdlWindow);
    }
    
    SDL_Quit();
}

void GraphicsDriver::InitInstance()
{
    VkResult vkResult;

    // Load Vulkan library and lookup core function addresses
    int vkVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
    if (vkVersion == 0) {
        throw Exception("gladLoaderLoadVulkan() failed");
    }

    Log(NOON_ANCHOR, "Vulkan {}.{}", GLAD_VERSION_MAJOR(vkVersion), GLAD_VERSION_MINOR(vkVersion));

    // 
    uint32_t availableLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
    if (availableLayerCount == 0) {
        throw Exception("vkEnumerateInstanceLayerProperties() failed, no available layers");
    }

    std::vector<VkLayerProperties> availableLayerList(availableLayerCount);
    vkResult = vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayerList.data());
    if (vkResult != VK_SUCCESS) {
        throw Exception("vkEnumerateInstanceLayerProperties() failed, {}", VkResultToString(vkResult));
    }

    Log(NOON_ANCHOR, "Available Vulkan Layers:");
    for (const auto& layer : availableLayerList) {
        Log(NOON_ANCHOR, "\t{}: {}", layer.layerName, layer.description);
    }
}

void GraphicsDriver::TermInstance()
{

}

void GraphicsDriver::InitSurface()
{

}

void GraphicsDriver::TermSurface()
{

}

void GraphicsDriver::InitLogicalDevice()
{

}

void GraphicsDriver::TermLogicalDevice()
{

}

void GraphicsDriver::InitAllocator()
{

}

void GraphicsDriver::TermAllocator()
{

}

void GraphicsDriver::InitSwapChain()
{

}

void GraphicsDriver::TermSwapChain()
{

}

void GraphicsDriver::ResetSwapChain()
{

}

void GraphicsDriver::InitRenderPass()
{

}

void GraphicsDriver::TermRenderPass()
{

}

string VkResultToString(VkResult vkResult)
{
    switch (vkResult) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:
            return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        default:
            return "Unknown";
    }
}

} // namespace noon