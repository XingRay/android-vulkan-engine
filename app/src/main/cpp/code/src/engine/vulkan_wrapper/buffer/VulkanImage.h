//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    class VulkanImage {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Format mImageFormat;
        uint32_t mMipLevels;

        uint32_t mWidth;
        uint32_t mHeight;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
        vk::ImageView mImageView;

    public:
        VulkanImage(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height);

        ~VulkanImage();

        [[nodiscard]]
        const vk::Image &getImage() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        const vk::ImageView &getImageView() const;

        [[nodiscard]]
        vk::Format getImageFormat() const;

        [[nodiscard]]
        uint32_t getMipLevels() const;

        [[nodiscard]]
        uint32_t getWidth() const;

        [[nodiscard]]
        uint32_t getHeight() const;

        void transitionImageLayout(const VulkanCommandPool &commandPool);

        void recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer);

        void copyFromBuffer(const VulkanCommandPool &vulkanCommandPool, vk::Buffer buffer);

        void recordCommandCopyFromBuffer(const vk::CommandBuffer &commandBuffer, vk::Buffer buffer);
    };

} // engine
