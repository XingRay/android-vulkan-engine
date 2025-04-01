//
// Created by leixing on 2025/4/1.
//

#pragma once

#include <cstdint>
#include <vector>

namespace engine {

    class VulkanBufferViewCreateInfo {
    private:
        // create buffer
        uint32_t mCapacity;
        std::vector<uint8_t> mData;
        uint32_t mOffset;
        uint32_t mRange;

    public:
        VulkanBufferViewCreateInfo(uint32_t capacity, const void *data, uint32_t size, uint32_t offset, uint32_t range);

        VulkanBufferViewCreateInfo(uint32_t capacity);

        VulkanBufferViewCreateInfo(const void *data, uint32_t size);

        VulkanBufferViewCreateInfo(uint32_t capacity, const void *data, uint32_t size);

        VulkanBufferViewCreateInfo(uint32_t capacity, uint32_t offset, uint32_t range);

        VulkanBufferViewCreateInfo(const void *data, uint32_t size, uint32_t offset, uint32_t range);

        template<class T>
        VulkanBufferViewCreateInfo(uint32_t capacity, std::vector<T> data, uint32_t offset, uint32_t range);

        template<class T>
        VulkanBufferViewCreateInfo(std::vector<T> data);

        template<class T>
        VulkanBufferViewCreateInfo(uint32_t capacity, std::vector<T> data);

        template<class T>
        VulkanBufferViewCreateInfo(std::vector<T> data, uint32_t offset, uint32_t range);

        ~VulkanBufferViewCreateInfo();
    };

} // engine
