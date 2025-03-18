//
// Created by leixing on 2025/3/10.
//

#include "VulkanPushConstantsBuffer.h"

namespace engine {
    VulkanPushConstantsBuffer::VulkanPushConstantsBuffer(uint32_t size)
            : mPushConstants(size) {

    }

    VulkanPushConstantsBuffer::~VulkanPushConstantsBuffer() = default;

    void VulkanPushConstantsBuffer::update(const void *data, uint32_t size) {
        std::memcpy(mPushConstants.data(), data, size);
    }
} // engine