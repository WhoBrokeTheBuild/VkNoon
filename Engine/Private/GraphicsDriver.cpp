#include <Noon/GraphicsDriver.hpp>
#include <Noon/Application.hpp>
#include <Noon/Exception.hpp>
#include <Noon/Noon.hpp>
#include <Noon/Log.hpp>

#include <SDL_vulkan.h>

NOON_DISABLE_WARNINGS()

    #define VMA_IMPLEMENTATION
    #include <vk_mem_alloc.h>

NOON_ENABLE_WARNINGS()

#include <set>

namespace noon {

using std::set;

NOON_API
GraphicsDriver::GraphicsDriver()
{
    InitWindow();
    InitInstance();
    InitSurface();
    InitDevice();
    InitAllocator();
    InitSwapChain();
}

NOON_API
GraphicsDriver::~GraphicsDriver()
{
    TermSwapChain();
    TermAllocator();
    TermDevice();
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

bool GraphicsDriver::HasLayerAvailable(const char * layer)
{
    return (_vkAvailableLayerMap.find(layer) != _vkAvailableLayerMap.end());
}

bool GraphicsDriver::HasInstanceExtensionAvailable(const char * extension)
{
    return (_vkAvailableInstanceExtensionMap.find(extension) != _vkAvailableInstanceExtensionMap.end());
}

bool GraphicsDriver::HasDeviceExtensionAvailable(const char * extension)
{
    return (_vkAvailableDeviceExtensionMap.find(extension) != _vkAvailableDeviceExtensionMap.end());
}

vector<const char *> GraphicsDriver::GetRequiredLayerList()
{
    vector<const char *> requiredLayerList = { };

    #if defined(NOON_BUILD_DEBUG)
    {
        if (HasLayerAvailable("VK_LAYER_KHRONOS_validation")) {
            requiredLayerList.push_back("VK_LAYER_KHRONOS_validation");
        }
        else {
            Log(NOON_ANCHOR, "Vulkan Layer 'VK_LAYER_KHRONOS_validation' not available");
        }
    }
    #endif

    return requiredLayerList;
}

vector<const char *> GraphicsDriver::GetRequiredDeviceExtensionList()
{
    vector<const char *> extensionList = { };

    if (HasDeviceExtensionAvailable(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME)) {
        extensionList.push_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
    }

    return extensionList;
}

vector<const char *> GraphicsDriver::GetRequiredInstanceExtensionList()
{
    SDL_bool sdlResult;
    
    uint32_t requiredExtensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, nullptr);

    vector<const char *> extensionList(requiredExtensionCount);
    sdlResult = SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &requiredExtensionCount, extensionList.data());
    if (!sdlResult) {
        throw Exception("SDL_Vulkan_GetInstanceExtensions failed, {}", SDL_GetError());
    }

    #if defined(NOON_BUILD_DEBUG)
    {
        if (HasInstanceExtensionAvailable(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
            extensionList.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    }
    #endif

    return extensionList;
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

static VKAPI_ATTR VkBool32 VKAPI_CALL _VulkanDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
    void * userData)
{
    if (strcmp(callbackData->pMessageIdName, "Loader Message") == 0) {
        return VK_FALSE;
    }

    const char * prefix = "????";

    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) > 0) {
        prefix = "PERF";
    }
    else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) > 0) {
        prefix = "ERRO";
    }
    else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) > 0) {
        prefix = "WARN";
    }
    else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) > 0) {
        prefix = "INFO";
    }
    else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) > 0) {
        prefix = "VERB";
    }

    Log("VkDebugUtilsMessenger",
        "[{}][{}] {}", 
        prefix,
        callbackData->pMessageIdName,
        callbackData->pMessage);

    for (uint32_t i = 0; i < callbackData->objectCount; ++i) {
        const auto& object = callbackData->pObjects[i];
        if (object.pObjectName) {
            Log("VkDebugUtilsMessenger", "\t\tObject #{}: '{}' Type {}, Value %p",
                i,
                object.pObjectName,
                object.objectType,
                object.objectHandle);
        }
    }
    
    for (uint32_t i = 0; i < callbackData->cmdBufLabelCount; ++i) {
        const auto& label = callbackData->pCmdBufLabels[i];
        if (label.pLabelName) {
            Log("VkDebugUtilsMessenger", "\t\tLabel #{}: '{}'",
                i,
                label.pLabelName);
        }
    }

    return VK_FALSE;
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

    uint32_t availableLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
    if (availableLayerCount == 0) {
        throw Exception("vkEnumerateInstanceLayerProperties() failed, no available layers");
    }

    vector<VkLayerProperties> availableLayerList(availableLayerCount);
    vkResult = vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayerList.data());
    if (vkResult != VK_SUCCESS) {
        throw Exception("vkEnumerateInstanceLayerProperties() failed, {}", VkResultToString(vkResult));
    }

    Log(NOON_ANCHOR, "Available Vulkan Layers:");
    for (auto layer : availableLayerList) {
        Log(NOON_ANCHOR, "\t{}: {}", layer.layerName, layer.description);
        _vkAvailableLayerMap.emplace(layer.layerName, layer);
    }

    const auto& requiredLayerList = GetRequiredLayerList();

    Log(NOON_ANCHOR, "Required Vulkan Layers:");
    for (auto layer : requiredLayerList) {
        Log(NOON_ANCHOR, "\t{}", layer);
    }

    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    if (availableExtensionCount == 0) {
        throw Exception("vkEnumerateInstanceExtensionProperties() failed, no extensions available");
    }

    vector<VkExtensionProperties> availableExtensionList(availableExtensionCount);
    vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensionList.data());
    if (vkResult != VK_SUCCESS) {
        throw Exception("vkEnumerateInstanceExtensionProperties() failed");
    }

    Log(NOON_ANCHOR, "Available Vulkan Instance Extensions:");
    for (auto extension : availableExtensionList) {
        Log(NOON_ANCHOR, "\t{}", extension.extensionName);
        _vkAvailableInstanceExtensionMap.emplace(extension.extensionName, extension);
    }

    const auto& requiredExtensionList = GetRequiredInstanceExtensionList();

    Log(NOON_ANCHOR, "Required Vulkan Instance Extensions:");
    for (auto extension : requiredExtensionList) {
        Log(NOON_ANCHOR, "\t{}", extension);
    }

    const auto& engineVersion = GetVersion();
    const auto& appVersion = Application::GetInstance()->GetVersion();
    const auto& appName = Application::GetInstance()->GetName();
    
    VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = appName.c_str(),
        .applicationVersion = VK_MAKE_VERSION(
            appVersion.Major,
            appVersion.Minor,
            appVersion.Patch
        ),
        .pEngineName = "Noon",
        .engineVersion = VK_MAKE_VERSION(
            engineVersion.Major,
            engineVersion.Minor,
            engineVersion.Patch
        ),
        .apiVersion = VK_API_VERSION_1_1,
    };

    VkInstanceCreateInfo instanceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = static_cast<uint32_t>(requiredLayerList.size()),
        .ppEnabledLayerNames = requiredLayerList.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensionList.size()),
        .ppEnabledExtensionNames = requiredExtensionList.data(),
    };
    
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    #if defined(NOON_BUILD_DEBUG)
        messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    #endif

    VkDebugUtilsMessageTypeFlagsEXT messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    _vkDebugMessengerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = messageSeverity,
        .messageType = messageType,
        .pfnUserCallback = _VulkanDebugMessageCallback,
        .pUserData = nullptr,
    };

    if (HasLayerAvailable("VK_LAYER_KHRONOS_validation")) {
        instanceCreateInfo.pNext = &_vkDebugMessengerCreateInfo;
    }

    vkResult = vkCreateInstance(&instanceCreateInfo, nullptr, &_vkInstance);
    if (vkResult != VK_SUCCESS) {
        throw Exception("vkCreateInstance() failed");
    }

    // Reload glad and lookup instance extension function addresses
    vkVersion = gladLoaderLoadVulkan(_vkInstance, nullptr, nullptr);
    if (vkVersion == 0) {
        throw Exception("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance");
    }

    InitDebugUtilsMessenger();
}

void GraphicsDriver::TermInstance()
{
    TermDebugUtilsMessenger();

    if (!_vkInstance) {
        vkDestroyInstance(_vkInstance, nullptr);
        _vkInstance = nullptr;
    }
}

void GraphicsDriver::InitDebugUtilsMessenger()
{
    VkResult vkResult;

    if (!vkCreateDebugUtilsMessengerEXT) {
        return;
    }

    vkResult = vkCreateDebugUtilsMessengerEXT(_vkInstance, &_vkDebugMessengerCreateInfo, nullptr, &_vkDebugMessenger);
    if (vkResult != VK_SUCCESS) {
        Log(NOON_ANCHOR, "vkCreateDebugUtilsMessengerEXT() failed");
        return;
    }
}

void GraphicsDriver::TermDebugUtilsMessenger()
{
    if (!_vkDebugMessenger) {
        return;
    }

    if (!vkDestroyDebugUtilsMessengerEXT) {
        Log(NOON_ANCHOR, "vkDestroyDebugUtilsMessengerEXT() is not bound");
        return;
    }

    vkDestroyDebugUtilsMessengerEXT(_vkInstance, _vkDebugMessenger, nullptr);
}

void GraphicsDriver::InitSurface()
{
    SDL_bool sdlResult;

    sdlResult = SDL_Vulkan_CreateSurface(_sdlWindow, _vkInstance, &_vkSurface);
    if (!sdlResult) {
        throw Exception("SDL_Vulkan_CreateSurface() failed, {}", SDL_GetError());
    }
}

void GraphicsDriver::TermSurface()
{
    if (_vkSurface) {
        vkDestroySurfaceKHR(_vkInstance, _vkSurface, nullptr);
        _vkSurface = nullptr;
    }
}

void GraphicsDriver::FindPhysicalDevice()
{
    VkResult vkResult;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw Exception("vkEnumeratePhysicalDevices() failed, no deviceList found");
    }

    vector<VkPhysicalDevice> deviceList(deviceCount);
    vkResult = vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, deviceList.data());
    if (vkResult != VK_SUCCESS) {
        throw Exception("vkEnumeratePhysicalDevices() failed");
    }

    for (const auto& device : deviceList) {
        vkGetPhysicalDeviceProperties(device, &_vkPhysicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(device, &_vkPhysicalDeviceFeatures);

        // TODO:
        // * Queue Families
        // * Device Extensions
        // * Swap Chain Support

        bool isSuitable = (
            _vkPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && _vkPhysicalDeviceFeatures.geometryShader
        );

        if (isSuitable) {
            _vkPhysicalDevice = device;
            break;
        }
    }

    if (_vkPhysicalDevice == VK_NULL_HANDLE) {
        throw Exception("No suitable vulkan physical device found");
    }

    Log(NOON_ANCHOR, "Physical Device Name: {}", _vkPhysicalDeviceProperties.deviceName);

    // Reload glad and lookup physical device extension function addresses
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, nullptr);
    if (vkVersion == 0) {
        throw Exception("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance and VkPhysicalDevice");
    }
}

void GraphicsDriver::InitDevice()
{
    VkResult vkResult;

    FindPhysicalDevice();
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount == 0) {
        throw Exception("vkGetPhysicalDeviceQueueFamilyProperties() failed, no queues found");
    }

    vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    uint32_t graphicsQueueFamilyIndex = UINT32_MAX;
    uint32_t presentQueueFamilyIndex = UINT32_MAX;

    uint32_t index = 0;
    for (const auto& queue : queueFamilyProperties) {
        string types;

        if ((queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) > 0) {
            types += "Graphics ";

            if (graphicsQueueFamilyIndex == UINT32_MAX) {
                graphicsQueueFamilyIndex = index;
                types.insert(types.end() - 1, '*');
            }
        }

        if ((queue.queueFlags & VK_QUEUE_COMPUTE_BIT) > 0) {
            types += "Compute ";
        }

        if ((queue.queueFlags & VK_QUEUE_TRANSFER_BIT) > 0) {
            types += "Transfer ";
        }

        if ((queue.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) > 0) {
            types += "SparseBinding ";
        }

        VkBool32 isPresentSupported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, index, _vkSurface, &isPresentSupported);

        if (isPresentSupported) {
            types += "Present ";

            if (presentQueueFamilyIndex == UINT32_MAX) {
                presentQueueFamilyIndex = index;
                types.insert(types.end() - 1, '*');
            }
        }

        Log(NOON_ANCHOR, "Queue #{}: {}", index, types);
        ++index;
    }

    if (graphicsQueueFamilyIndex == UINT32_MAX) {
        throw Exception("No suitable graphics queue found");
    }

    if (presentQueueFamilyIndex == UINT32_MAX) {
        throw Exception("No suitable present queue found");
    }

    const float queuePriorities = 1.0f;

    vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
    set<uint32_t> queueFamilyIndexSet = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };

    for (auto index : queueFamilyIndexSet) {
        queueCreateInfoList.push_back(
            VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = index,
                .queueCount = 1,
                .pQueuePriorities = &queuePriorities,
            }
        );
    }

    VkPhysicalDeviceFeatures requiredDeviceFeatures = {
        // TODO
    };

    const auto& requiredLayerList = GetRequiredLayerList();

    uint32_t availableExtensionCount = 0;
    vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &availableExtensionCount, nullptr);

    if (availableExtensionCount == 0) {
        throw Exception("vkEnumerateDeviceExtensionProperties() failed, no extensions available");
    }

    vector<VkExtensionProperties> availableExtensionList(availableExtensionCount);
    vkResult = vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &availableExtensionCount, availableExtensionList.data());
    if (vkResult != VK_SUCCESS) {
        throw Exception("vkEnumerateDeviceExtensionProperties() failed");
    }

    Log(NOON_ANCHOR, "Available Vulkan Device Extensions:");
    for (const auto& extension : availableExtensionList) {
        Log(NOON_ANCHOR, "\t{}", extension.extensionName);
        _vkAvailableDeviceExtensionMap.emplace(extension.extensionName, extension);
    }

    const auto& requiredExtensionList = GetRequiredDeviceExtensionList();
    
    Log(NOON_ANCHOR, "Required Vulkan Device Extensions:");
    for (auto extension : requiredExtensionList) {
        Log(NOON_ANCHOR, "\t{}", extension);
    }

    VkDeviceCreateInfo deviceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoList.size()),
        .pQueueCreateInfos = queueCreateInfoList.data(),
        .enabledLayerCount = static_cast<uint32_t>(requiredLayerList.size()),
        .ppEnabledLayerNames = requiredLayerList.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensionList.size()),
        .ppEnabledExtensionNames = requiredExtensionList.data(),
        .pEnabledFeatures = &requiredDeviceFeatures,
    };

    vkResult = vkCreateDevice(_vkPhysicalDevice, &deviceCreateInfo, nullptr, &_vkDevice);
    if (vkResult != VK_SUCCESS) {
        throw Exception(NOON_ANCHOR, "vkCreateDevice() failed");
    }

    // Reload glad and lookup device extension function addresses
    int vkVersion = gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, _vkDevice);
    if (vkVersion == 0) {
        throw Exception("gladLoaderLoadVulkan() failed, unable to reload symbols with VkInstance, VkPhysicalDevice and VkDevice");
    }
    
    vkGetDeviceQueue(_vkDevice, graphicsQueueFamilyIndex, 0, &_vkGraphicsQueue);
    vkGetDeviceQueue(_vkDevice, presentQueueFamilyIndex, 0, &_vkPresentQueue);
}

void GraphicsDriver::TermDevice()
{
    if (_vkDevice) {
        vkDestroyDevice(_vkDevice, nullptr);
        _vkDevice = nullptr;
    }
}

void GraphicsDriver::InitAllocator()
{
    VkResult vkResult;

    VmaAllocatorCreateFlags allocatorCreateFlags = 0;

    if (HasDeviceExtensionAvailable(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME)) {
        allocatorCreateFlags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    }
    
    VmaAllocatorCreateInfo allocatorCreateInfo = {
        .flags = allocatorCreateFlags,
        .physicalDevice = _vkPhysicalDevice,
        .device = _vkDevice,
        // .preferredLargeHeapBlockSize = ,
        // .pAllocationCallbacks = ,
        // .pDeviceMemoryCallbacks = ,
        .instance = _vkInstance,
        .vulkanApiVersion = VK_API_VERSION_1_2,
    };

    vkResult = vmaCreateAllocator(&allocatorCreateInfo, &_vmaAllocator);
    if (vkResult != VK_SUCCESS) {
        throw Exception("vmaCreateAllocator() failed");
    }
}

void GraphicsDriver::TermAllocator()
{
    if (_vmaAllocator) {
        vmaDestroyAllocator(_vmaAllocator);
        _vmaAllocator = nullptr;
    }
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