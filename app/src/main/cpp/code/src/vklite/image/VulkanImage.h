//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/command/VulkanCommandPool.h"
#include "vklite/buffer/host_visible/VulkanStagingBuffer.h"
#include "VulkanImageInterface.h"

namespace vklite {

    class VulkanImage : public VulkanImageInterface {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Format mImageFormat;
        uint32_t mMipLevels;

        uint32_t mWidth;
        uint32_t mHeight;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
        vk::ImageView mImageView;

        std::unique_ptr<VulkanStagingBuffer> mVulkanStagingBuffer;

    public:
        VulkanImage(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, vk::Format format);

        ~VulkanImage();

        [[nodiscard]]
        const vk::Image &getImage() const override;

        [[nodiscard]]
        const vk::ImageView &getImageView() const override;

        [[nodiscard]]
        uint32_t getMipLevels() const override;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        vk::Format getImageFormat() const;

        [[nodiscard]]
        uint32_t getWidth() const;

        [[nodiscard]]
        uint32_t getHeight() const;

        void transitionImageLayout(const VulkanCommandPool &commandPool) override;

        void recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer);

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) override;

        void recordCommandCopyFromBuffer(const vk::CommandBuffer &commandBuffer, vk::Buffer buffer);

        void generateMipmaps(const VulkanCommandPool &vulkanCommandPool);

        void recordCommandGenerateMipmaps(const vk::CommandBuffer &commandBuffer);
    };

} // engine
