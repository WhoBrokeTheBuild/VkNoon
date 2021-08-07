#ifndef NOON_GRAPHICS_DRIVER_HPP
#define NOON_GRAPHICS_DRIVER_HPP

#include <Noon/Config.hpp>
#include <Noon/String.hpp>

#include <SDL.h>
#include <glad/vulkan.h>

NOON_DISABLE_WARNINGS()

    #include <vk_mem_alloc.h>

NOON_ENABLE_WARNINGS()

#include <vector>
#include <unordered_map>

namespace noon {

using std::vector;
using std::unordered_map;

class NOON_API GraphicsDriver
{
public:

    NOON_DISALLOW_COPY_AND_ASSIGN(GraphicsDriver);

    GraphicsDriver();

    ~GraphicsDriver();

    void ProcessEvents();
    
    void Render();

protected:

    bool HasLayerAvailable(const char * layer);

    bool HasInstanceExtensionAvailable(const char * extension);

    bool HasDeviceExtensionAvailable(const char * extension);

    virtual vector<const char *> GetRequiredLayerList();

    virtual vector<const char *> GetRequiredInstanceExtensionList();

    virtual vector<const char *> GetRequiredDeviceExtensionList();

private:

    void InitWindow();

    void TermWindow();

    void InitInstance();

    void TermInstance();

    void InitDebugUtilsMessenger();

    void TermDebugUtilsMessenger();

    void InitSurface();

    void TermSurface();

    void FindPhysicalDevice();

    void InitDevice();

    void TermDevice();

    void InitAllocator();

    void TermAllocator();

    void InitSwapChain();

    void TermSwapChain();

    void ResetSwapChain();

    void InitRenderPass();

    void TermRenderPass();

    static GraphicsDriver * _Instance;

    SDL_Window * _sdlWindow;

    unordered_map<string, VkLayerProperties> _vkAvailableLayerMap;

    unordered_map<string, VkExtensionProperties> _vkAvailableInstanceExtensionMap;

    unordered_map<string, VkExtensionProperties> _vkAvailableDeviceExtensionMap;
    
    VkInstance _vkInstance = nullptr;

    VkDebugUtilsMessengerCreateInfoEXT _vkDebugMessengerCreateInfo;

    VkDebugUtilsMessengerEXT _vkDebugMessenger = nullptr;

    VkSurfaceKHR _vkSurface = nullptr;

    VkPhysicalDeviceProperties _vkPhysicalDeviceProperties;
    
    VkPhysicalDeviceFeatures _vkPhysicalDeviceFeatures;

    VkPhysicalDevice _vkPhysicalDevice = nullptr;

    VkDevice _vkDevice = nullptr;

    VkQueue _vkGraphicsQueue = nullptr;
    
    VkQueue _vkPresentQueue = nullptr;

    VmaAllocator _vmaAllocator;

}; // class GraphicsDriver

string VkResultToString(VkResult vkResult);

} // namespace noon

#endif // NOON_GRAPHICS_DRIVER_HPP