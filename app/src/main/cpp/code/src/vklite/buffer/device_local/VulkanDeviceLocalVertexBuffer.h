//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "VulkanDeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/VulkanStagingBuffer.h"
#include "vklite/command/VulkanCommandPool.h"

namespace vklite {

    class VulkanDeviceLocalVertexBuffer {
    private:
        VulkanDeviceLocalBuffer mVertexBuffer;
        VulkanStagingBuffer mStagingBuffer;

    public:
        VulkanDeviceLocalVertexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanDeviceLocalVertexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        template<class T>
        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<T> &data) {
            recordCommandUpdate(commandBuffer, data.data(), data.size() * sizeof(T));
        }

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size);

        template<class T>
        void update(const VulkanCommandPool &vulkanCommandPool, const std::vector<T> &data) {
            update(vulkanCommandPool, data.data(), data.size() * sizeof(T));
        }

    };

} // engine
