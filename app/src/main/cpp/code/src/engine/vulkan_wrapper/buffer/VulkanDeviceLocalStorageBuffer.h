//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"


#include "engine/vulkan_wrapper/buffer/VulkanDeviceLocalBuffer.h"
#include "engine/vulkan_wrapper/buffer/VulkanStagingBuffer.h"

namespace engine {

    class VulkanDeviceLocalStorageBuffer {
    private:
        const VulkanDevice &mVulkanDevice;

        VulkanStagingBuffer mStagingBuffer;
        VulkanDeviceLocalBuffer mStorageBuffer;

    public:
        VulkanDeviceLocalStorageBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlags additionalUsageFlags = vk::BufferUsageFlags{});

        ~VulkanDeviceLocalStorageBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size);

        [[nodiscard]]
        std::vector<vk::DescriptorBufferInfo> createDescriptorBufferInfos();
    };

} // engine
