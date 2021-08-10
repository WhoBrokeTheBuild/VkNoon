#include <Noon/Buffer.hpp>
#include <Noon/Exception.hpp>
#include <Noon/Application.hpp>

#include <cstring>
#include <cassert>

namespace noon {

Buffer::Buffer(VkDeviceSize size, uint8_t * data, VkBufferUsageFlags bufferUsageFlags, VmaMemoryUsage memoryUsage)
    : _size(size)
    , _vkBufferUsageFlags(bufferUsageFlags)
    , _vmaMemoryUsage(memoryUsage)
{
    VkResult vkResult;

    auto gfx = Application::GetInstance()->GetGraphicsDriver();

    // TODO: Investigate
    // if (_vmaMemoryUsage == VMA_MEMORY_USAGE_GPU_ONLY && !data) {
    //     throw Exception("Attempting to create an empty buffer with VMA_MEMORY_USAGE_GPU_ONLY");
    // }

    if (_vmaMemoryUsage == VMA_MEMORY_USAGE_GPU_ONLY) {
        VkBuffer stagingBuffer = VK_NULL_HANDLE;
        VmaAllocation stagingAllocation = VK_NULL_HANDLE;

        if (data) {
            // If we are uploading to a GPU only buffer, we must use a staging buffer
            VkBufferCreateInfo stagingBufferCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .size = _size,
                .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            };

            VmaAllocationCreateInfo stagingAllocationCreateInfo = {
                .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
                .usage = VMA_MEMORY_USAGE_CPU_ONLY,
            };
            
            VmaAllocationInfo stagingAllocationInfo;

            vkResult = vmaCreateBuffer(
                gfx->GetAllocator(),
                &stagingBufferCreateInfo,
                &stagingAllocationCreateInfo,
                &stagingBuffer,
                &stagingAllocation,
                &stagingAllocationInfo);
        
            if (vkResult != VK_SUCCESS) {
                throw Exception("vmaCreateBuffer() failed, unable to create staging buffer");
            }

            _vkBufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        
        VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = _size,
            .usage = _vkBufferUsageFlags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VmaAllocationCreateInfo allocationCreateInfo = {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = _vmaMemoryUsage,
        };
        
        vkResult = vmaCreateBuffer(
            gfx->GetAllocator(),
            &bufferCreateInfo,
            &allocationCreateInfo,
            &_vkBuffer,
            &_vmaAllocation,
            nullptr);
        
        if (vkResult != VK_SUCCESS) {
            throw Exception("vmaCreateBuffer() failed");
        }

        if (data) {
            gfx->CopyBuffer(stagingBuffer, _vkBuffer, _size);

            vkDestroyBuffer(gfx->GetDevice(), stagingBuffer, nullptr);
            vmaFreeMemory(gfx->GetAllocator(), stagingAllocation);
        }
    }
    else {
        VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = _size,
            .usage = _vkBufferUsageFlags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        VmaAllocationCreateInfo allocationCreateInfo = {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = _vmaMemoryUsage,
        };

        VmaAllocationInfo allocationInfo;

        vkResult = vmaCreateBuffer(
            gfx->GetAllocator(),
            &bufferCreateInfo,
            &allocationCreateInfo,
            &_vkBuffer,
            &_vmaAllocation,
            &allocationInfo);
        
        if (vkResult != VK_SUCCESS) {
            throw Exception("vmaCreateBuffer() failed");
        }

        _mappedBufferMemory = static_cast<uint8_t *>(allocationInfo.pMappedData);

        bool upload = (
            _vmaMemoryUsage == VMA_MEMORY_USAGE_CPU_ONLY ||
            _vmaMemoryUsage == VMA_MEMORY_USAGE_CPU_TO_GPU
        );

        if (data && upload) {
            memcpy(_mappedBufferMemory, data, _size);            
        }
    }
}

Buffer::~Buffer()
{
    auto gfx = Application::GetInstance()->GetGraphicsDriver();

    if (_mappedBufferMemory) {
        vmaUnmapMemory(gfx->GetAllocator(), _vmaAllocation);
        _mappedBufferMemory = nullptr;
    }

    vkDestroyBuffer(gfx->GetDevice(), _vkBuffer, nullptr);
    _vkBuffer = VK_NULL_HANDLE;

    vmaFreeMemory(gfx->GetAllocator(), _vmaAllocation);
    _vmaAllocation = VK_NULL_HANDLE;
}

void Buffer::ReadFrom(VkDeviceSize offset, VkDeviceSize length, uint8_t * data)
{
    assert(_vmaMemoryUsage == VMA_MEMORY_USAGE_GPU_TO_CPU);
    assert(_mappedBufferMemory);
    assert(_size <= offset + length);

    memcpy(data, _mappedBufferMemory + offset, length);
}

void Buffer::WriteTo(VkDeviceSize offset, VkDeviceSize length, const uint8_t * data)
{
    assert(_vmaMemoryUsage == VMA_MEMORY_USAGE_CPU_ONLY || _vmaMemoryUsage == VMA_MEMORY_USAGE_CPU_TO_GPU);
    assert(_mappedBufferMemory);
    assert(_size <= offset + length);

    memcpy(_mappedBufferMemory + offset, data, length);
}


} // namespace noon