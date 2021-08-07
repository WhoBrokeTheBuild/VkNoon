#ifndef NOON_GRAPHICS_DRIVER_HPP
#define NOON_GRAPHICS_DRIVER_HPP

#include <Noon/Config.hpp>
#include <Noon/String.hpp>

#include <SDL.h>
#include <glad/vulkan.h>

#include <vector>
#include <unordered_map>

namespace noon {

class NOON_API GraphicsDriver
{
public:

    GraphicsDriver();

    ~GraphicsDriver();

    void ProcessEvents();
    
    void Render();

private:

    bool IsDeviceSuitable(const VkPhysicalDevice device);

    std::vector<const char *> GetRequiredLayers();

    std::vector<const char *> GetRequiredDeviceExtensions();

    std::vector<const char *> GetRequiredInstanceExtensions();

    void GetPhysicalDevice();

    void InitWindow();

    void TermWindow();

    void InitInstance();

    void TermInstance();

    void InitSurface();

    void TermSurface();

    void InitLogicalDevice();

    void TermLogicalDevice();

    void InitAllocator();

    void TermAllocator();

    void InitSwapChain();

    void TermSwapChain();

    void ResetSwapChain();

    void InitRenderPass();

    void TermRenderPass();

    static GraphicsDriver * _Instance;

    SDL_Window * _sdlWindow;

    std::unordered_map<string, VkLayerProperties> _vkAvailableLayers;

    std::unordered_map<string, VkExtensionProperties> _vkAvailableInstanceExtensions;

    std::unordered_map<string, VkExtensionProperties> _vkAvailableDeviceExtensions;
    
    VkInstance _vkInstance = nullptr;

    VkDebugUtilsMessengerEXT _vkDebugMessenger = nullptr;

    VkSurfaceKHR _vkSurface = nullptr;

    VkPhysicalDeviceProperties _vkPhysicalDeviceProperties;
    
    VkPhysicalDeviceFeatures _vkPhysicalDeviceFeatures;

    VkPhysicalDevice _vkPhysicalDevice = nullptr;


}; // class GraphicsDriver

string VkResultToString(VkResult vkResult);

} // namespace noon

#endif // NOON_GRAPHICS_DRIVER_HPP