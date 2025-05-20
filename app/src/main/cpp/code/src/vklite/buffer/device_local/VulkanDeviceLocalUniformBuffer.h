//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "VulkanDeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/VulkanStagingBuffer.h"
#include "vklite/command/VulkanCommandPool.h"
#include "vklite/buffer/VulkanBufferInterface.h"

namespace vklite {

    class VulkanDeviceLocalUniformBuffer : public VulkanBufferInterface {
    private:
        VulkanDeviceLocalBuffer mUniformBuffer;
        VulkanStagingBuffer mStagingBuffer;

    public:
        VulkanDeviceLocalUniformBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanDeviceLocalUniformBuffer() override;

        [[nodiscard]]
        vk::DeviceSize getSize() const override;

        [[nodiscard]]
        const vk::Buffer &getBuffer() const override;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        template<class T>
        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<T> &data) {
            recordCommandUpdate(commandBuffer, data.data(), data.size() * sizeof(T));
        }

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) override;

        template<class T>
        void update(const VulkanCommandPool &vulkanCommandPool, const std::vector<T> &data) {
            update(vulkanCommandPool, data.data(), data.size() * sizeof(T));
        }

        std::vector<vk::DescriptorBufferInfo> createDescriptorBufferInfos();
    };

} // engine
