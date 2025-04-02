//
// Created by leixing on 2025/3/31.
//

#include "VulkanBufferViewConfigure.h"

namespace engine {

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(const std::shared_ptr<VulkanBufferView> &bufferView)
            : mBufferView(bufferView), mBufferViewCreateInfo(nullptr) {

    }

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(uint32_t capacity, const void *data, uint32_t size)
            : mBufferView(nullptr), mBufferViewCreateInfo(std::make_unique<VulkanBufferViewCreateInfo>(capacity, data, size)) {
    }

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(uint32_t capacity)
            : VulkanBufferViewConfigure(capacity, nullptr, 0) {

    }

    VulkanBufferViewConfigure::~VulkanBufferViewConfigure() = default;

} // engine