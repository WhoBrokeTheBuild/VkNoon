#ifndef NOON_BUFFER_HPP
#define NOON_BUFFER_HPP

#include <Noon/Config.hpp>
#include <Noon/GraphicsDriver.hpp>

#include <cstdint>

namespace noon {

class Buffer
{
public:

    NOON_DISALLOW_COPY_AND_ASSIGN(Buffer)

    Buffer(VkDeviceSize size, uint8_t * data, VkBufferUsageFlags bufferUsageFlags, VmaMemoryUsage memoryUsage);

    ~Buffer();

    inline size_t GetSize() const {
        return _size;
    }

    inline VkBufferUsageFlags GetBufferUsageFlags() const {
        return _vkBufferUsageFlags;
    }

    inline VmaMemoryUsage GetMemoryUsage() const {
        return _vmaMemoryUsage;
    }

    inline bool IsMapped() const {
        return (_mappedBufferMemory != nullptr);
    }

    void ReadFrom(VkDeviceSize offset, VkDeviceSize length, uint8_t * data);

    void WriteTo(VkDeviceSize offset, VkDeviceSize length, const uint8_t * data);

private:

    VkDeviceSize _size;

    VkBufferUsageFlags _vkBufferUsageFlags;

    VmaMemoryUsage _vmaMemoryUsage;

    uint8_t * _mappedBufferMemory = nullptr;

    VkBuffer _vkBuffer = VK_NULL_HANDLE;

    VmaAllocation _vmaAllocation = VK_NULL_HANDLE;

}; // class GraphicsBuffer

} // namespace noon

#endif // NOON_BUFFER_HPP