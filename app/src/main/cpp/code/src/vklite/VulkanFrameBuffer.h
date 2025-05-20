//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/swapchain/VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "vklite/command/VulkanCommandPool.h"

namespace vklite {

    class VulkanFrameBuffer {
    private:
        const VulkanDevice &mDevice;

        vk::Image mColorImage;
        vk::DeviceMemory mColorDeviceMemory;
        vk::ImageView mColorImageView;

        vk::Image mDepthImage;
        vk::DeviceMemory mDepthDeviceMemory;
        vk::ImageView mDepthImageView;

        std::vector<vk::Framebuffer> mFrameBuffers;

    public:
        VulkanFrameBuffer(const VulkanDevice &device, const VulkanSwapchain &vulkanSwapchain, const VulkanRenderPass &vulkanRenderPass, const VulkanCommandPool &commandPool);

        ~VulkanFrameBuffer();

        [[nodiscard]]
        const std::vector<vk::Framebuffer> &getFrameBuffers() const;

    private:

    };

} // engine
