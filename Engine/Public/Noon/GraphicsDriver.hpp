#ifndef NOON_GRAPHICS_DRIVER_HPP
#define NOON_GRAPHICS_DRIVER_HPP

#include <Noon/Config.hpp>
#include <Noon/Containers.hpp>
#include <Noon/Math.hpp>
#include <Noon/String.hpp>
#include <Noon/ShaderGlobals.hpp>
#include <Noon/ShaderTransform.hpp>

#include <SDL.h>
#include <glad/vulkan.h>

NOON_DISABLE_WARNINGS()

    #include <vk_mem_alloc.h>

NOON_ENABLE_WARNINGS()

namespace noon {

class NOON_API GraphicsDriver
{
public:

    NOON_DISALLOW_COPY_AND_ASSIGN(GraphicsDriver);

    GraphicsDriver();

    virtual ~GraphicsDriver();

    inline String GetWindowTitle() const {
        return _windowTitle;
    }

    void SetWindowTitle(const String& title);

    inline Vec2i GetWindowSize() const {
        return _windowSize;
    }

    void SetWindowSize(const Vec2i& size);

    inline unsigned GetBackbufferCount() const {
        return _backbufferCount;
    }

    void SetBackbufferCount(unsigned backbufferCount);

    inline VkDevice GetDevice() const {
        return _vkDevice;
    }

    inline VmaAllocator GetAllocator() const {
        return _vmaAllocator;
    }

    void ProcessEvents();
    
    void Render();

    // TODO: Move
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

protected:

    void UpdateShaderGlobals();

    bool HasLayerAvailable(const char * layer);

    bool HasInstanceExtensionAvailable(const char * extension);

    bool HasDeviceExtensionAvailable(const char * extension);

    virtual List<const char *> GetRequiredLayerList();

    virtual List<const char *> GetRequiredInstanceExtensionList();

    virtual List<const char *> GetRequiredDeviceExtensionList();

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

    void InitSyncObjects();

    void TermSyncObjects();

    void InitSwapChain();

    void TermSwapChain();

    void ResetSwapChain();

    void InitDepthBuffer();

    void TermDepthBuffer();

    void InitRenderPass();

    void TermRenderPass();

    void InitDescriptorPool();

    void TermDescriptorPool();

    void InitPipelineLayout();

    void TermPipelineLayout();

    void InitFramebuffers();

    void TermFramebuffers();

    void InitCommandBuffers();

    void TermCommandBuffers();

    void FillCommandBuffers();

    static GraphicsDriver * _Instance;

    SDL_Window * _sdlWindow;

    String _windowTitle;

    Vec2i _windowSize = { 640, 480 };

    unsigned _backbufferCount = 2;

    unsigned _backbufferIndex = 0;

    Map<String, VkLayerProperties> _vkAvailableLayerMap;

    Map<String, VkExtensionProperties> _vkAvailableInstanceExtensionMap;

    Map<String, VkExtensionProperties> _vkAvailableDeviceExtensionMap;
    
    VkInstance _vkInstance = VK_NULL_HANDLE;

    VkDebugUtilsMessengerCreateInfoEXT _vkDebugMessengerCreateInfo;

    VkDebugUtilsMessengerEXT _vkDebugMessenger = VK_NULL_HANDLE;

    VkSurfaceKHR _vkSurface = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties _vkPhysicalDeviceProperties;
    
    VkPhysicalDeviceFeatures _vkPhysicalDeviceFeatures;

    VkPhysicalDevice _vkPhysicalDevice = VK_NULL_HANDLE;

    VkDevice _vkDevice = VK_NULL_HANDLE;

    uint32_t _vkGraphicsQueueFamilyIndex;

    uint32_t _vkPresentQueueFamilyIndex;

    VkQueue _vkGraphicsQueue = VK_NULL_HANDLE;
    
    VkQueue _vkPresentQueue = VK_NULL_HANDLE;

    VmaAllocator _vmaAllocator = VK_NULL_HANDLE;

    VkFormat _vkSwapChainImageFormat;

    VkExtent2D _vkSwapChainExtent;

    VkSwapchainKHR _vkSwapChain = VK_NULL_HANDLE;

    List<VkImage> _vkSwapChainImageList;

    List<VkImageView> _vkSwapChainImageViewList;

    VkFormat _vkDepthImageFormat;

    VkImage _vkDepthImage = VK_NULL_HANDLE;

    VmaAllocation _vmaDepthImageAllocation = VK_NULL_HANDLE;

    VkImageView _vkDepthImageView = VK_NULL_HANDLE;

    VkRenderPass _vkRenderPass = VK_NULL_HANDLE;

    VkDescriptorPool _vkDescriptorPool = VK_NULL_HANDLE;

    List<VkDescriptorSetLayout> _vkDescriptorSetLayoutList;

    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;

    List<VkFramebuffer> _vkFramebufferList;

    VkCommandPool _vkCommandPool = VK_NULL_HANDLE;

    List<VkCommandBuffer> _vkCommandBufferList;

    List<VkSemaphore> _vkImageAvailableSemaphoreList;

    List<VkSemaphore> _vkRenderingFinishedSemaphoreList;

    List<VkFence> _vkInFlightFenceList;

    List<VkFence> _vkImageInFlightList;

    List<VkBuffer> _vkUniformBufferList;

}; // class GraphicsDriver

String VkResultToString(VkResult vkResult);

String VkFormatToString(VkFormat vkFormat);

String VkColorSpaceToString(VkColorSpaceKHR vkColorSpace);

String VkPresentModeToString(VkPresentModeKHR vkPresentMode);

} // namespace noon

#endif // NOON_GRAPHICS_DRIVER_HPP