//
// Created by leixing on 2025/4/1.
//

#include "VulkanBufferViewCreateInfo.h"

namespace engine {

    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(uint32_t capacity, const void *data, uint32_t size, uint32_t offset, uint32_t range)
            : mCapacity(capacity), mOffset(offset), mRange(range) {
        mData.reserve(size);
        const uint8_t *byteData = static_cast<const uint8_t *>(data);
        // 深拷贝数据
        mData.assign(byteData, byteData + size);
    }

    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(uint32_t capacity)
            : VulkanBufferViewCreateInfo(capacity, nullptr, 0, 0, capacity) {}

    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(const void *data, uint32_t size)
            : VulkanBufferViewCreateInfo(size, data, size, 0, size) {}

    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(uint32_t capacity, const void *data, uint32_t size)
            : VulkanBufferViewCreateInfo(capacity, data, size, 0, size) {}

    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(uint32_t capacity, uint32_t offset, uint32_t range)
            : VulkanBufferViewCreateInfo(capacity, nullptr, 0, offset, range) {}

    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(const void *data, uint32_t size, uint32_t offset, uint32_t range)
            : VulkanBufferViewCreateInfo(size, data, size, offset, range) {}

    template<class T>
    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(uint32_t capacity, std::vector<T> data, uint32_t offset, uint32_t range)
            : VulkanBufferViewCreateInfo(capacity, data.data(), data.size() * sizeof(T), offset, range) {}

    template<class T>
    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(std::vector<T> data)
            : VulkanBufferViewCreateInfo(data.size() * sizeof(T), data.data(), data.size() * sizeof(T), 0, data.size() * sizeof(T)) {}

    template<class T>
    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(uint32_t capacity, std::vector<T> data)
            : VulkanBufferViewCreateInfo(capacity, data.data(), data.size() * sizeof(T), 0, data.size() * sizeof(T)) {}

    template<class T>
    VulkanBufferViewCreateInfo::VulkanBufferViewCreateInfo(std::vector<T> data, uint32_t offset, uint32_t range)
            : VulkanBufferViewCreateInfo(data.size() * sizeof(T), data.data(), data.size() * sizeof(T), offset, range) {}

    VulkanBufferViewCreateInfo::~VulkanBufferViewCreateInfo() = default;

} // engine