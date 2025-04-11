//
// Created by leixing on 2025/3/10.
//

#include "engine/vulkan_wrapper/push_constants/VulkanPushConstants.h"

namespace engine {
    VulkanPushConstants::VulkanPushConstants(uint32_t size)
            : mPushConstants(size) {

    }

    VulkanPushConstants::~VulkanPushConstants() = default;

    void VulkanPushConstants::update(const void *data, uint32_t size) {
        std::memcpy(mPushConstants.data(), data, size);
    }
} // engine