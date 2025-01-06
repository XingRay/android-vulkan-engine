//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanCommandPool.h"

namespace engine {

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
