//
// Created by leixing on 2024/12/30.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
//#include "vklite/vulkan_wrapper/VulkanSwapchain.h"
//#include "vklite/vulkan_wrapper/VulkanRenderPass.h"

#include <functional>

namespace vklite {

    class VulkanCommandPool {
    private:
        const VulkanDevice &mDevice;

        vk::CommandPool mCommandPool;

        std::vector<vk::CommandBuffer> mCommandBuffers;

    public:
        VulkanCommandPool(const VulkanDevice &device, uint32_t commandBufferCount);

        ~VulkanCommandPool();

        const vk::CommandPool &getCommandPool() const;

        const std::vector<vk::CommandBuffer> &getCommandBuffers() const;

        vk::CommandBuffer allocateCommand() const;

        std::vector<vk::CommandBuffer> allocateCommands(uint32_t count) const;

        void submitOneTimeCommand(const std::function<void(const vk::CommandBuffer &)> &command) const;

        void submitCommand(const vk::CommandBuffer &commandBuffer) const;

        static void recordCommand(const vk::CommandBuffer &commandBuffer, vk::CommandBufferUsageFlagBits usage, const std::function<void(const vk::CommandBuffer &)> &command);
    };

} // engine
