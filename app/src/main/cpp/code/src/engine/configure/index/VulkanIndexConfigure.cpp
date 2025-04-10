//
// Created by leixing on 2025/3/22.
//

#include "VulkanIndexConfigure.h"

namespace engine {

    VulkanIndexConfigure::VulkanIndexConfigure() {

    }

    VulkanIndexConfigure::~VulkanIndexConfigure() {

    }

    VulkanIndexConfigure &VulkanIndexConfigure::setIndexBuffer(uint32_t capacity) {
        mCreateBufferCapacity = capacity;
        return *this;
    }

    VulkanIndexConfigure &VulkanIndexConfigure::setIndexBuffer(uint32_t capacity, std::vector<uint32_t> &&indices) {
        mCreateBufferCapacity = capacity;
        mCreateBufferIndices = std::move(indices);
        return *this;
    }

    VulkanIndexConfigure &VulkanIndexConfigure::setIndexBuffer(std::vector<uint32_t> &&indices) {
        mCreateBufferCapacity = indices.size() * sizeof(uint32_t);
        mCreateBufferIndices = std::move(indices);
        return *this;
    }

    VulkanIndexConfigure &VulkanIndexConfigure::setIndexBuffer(const std::shared_ptr<VulkanDeviceLocalIndexBuffer>& indexBuffer) {
        mIndexBuffer = indexBuffer;
        return *this;
    }

    std::shared_ptr<VulkanDeviceLocalIndexBuffer> VulkanIndexConfigure::createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const{
        if (mIndexBuffer != nullptr) {
            return mIndexBuffer;
        }

        if (mCreateBufferCapacity == 0) {
            return nullptr;
        }

        std::shared_ptr<VulkanDeviceLocalIndexBuffer> buffer = std::make_shared<VulkanDeviceLocalIndexBuffer>(vulkanDevice, mCreateBufferCapacity);
        if (!mCreateBufferIndices.empty()) {
            buffer->update(commandPool, mCreateBufferIndices);
        }

        return buffer;
    }

} // engine