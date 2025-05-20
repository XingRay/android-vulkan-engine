//
// Created by leixing on 2024/12/30.
//

#include "VulkanCommandPool.h"
#include "vklite/Log.h"

namespace vklite {

    VulkanCommandPool::VulkanCommandPool(const VulkanDevice &device, uint32_t commandBufferCount)
            : mDevice(device) {

        vk::CommandPoolCreateInfo commandPoolCreateInfo{};
        uint32_t queueFamilyIndex = device.getGraphicQueueFamilyIndex();
        commandPoolCreateInfo
                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(queueFamilyIndex);

        mCommandPool = device.getDevice().createCommandPool(commandPoolCreateInfo);

        vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo
                .setCommandPool(mCommandPool)
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandBufferCount(commandBufferCount);

        mCommandBuffers = mDevice.getDevice().allocateCommandBuffers(commandBufferAllocateInfo);
    }

    VulkanCommandPool::~VulkanCommandPool() {
        LOG_D("VulkanCommandPool::~VulkanCommandPool()");
        mDevice.getDevice().freeCommandBuffers(mCommandPool, mCommandBuffers);
        mDevice.getDevice().destroy(mCommandPool);
    }

    const vk::CommandPool &VulkanCommandPool::getCommandPool() const {
        return mCommandPool;
    }

    const std::vector<vk::CommandBuffer> &VulkanCommandPool::getCommandBuffers() const {
        return mCommandBuffers;
    }

    vk::CommandBuffer VulkanCommandPool::allocateCommand() const {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandPool(mCommandPool)
                .setCommandBufferCount(1);

        return mDevice.getDevice().allocateCommandBuffers(commandBufferAllocateInfo)[0];
    }

    std::vector<vk::CommandBuffer> VulkanCommandPool::allocateCommands(uint32_t count) const {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandPool(mCommandPool)
                .setCommandBufferCount(count);

        return mDevice.getDevice().allocateCommandBuffers(commandBufferAllocateInfo);
    }

    void VulkanCommandPool::submitCommand(const vk::CommandBuffer &commandBuffer) const {
        vk::SubmitInfo submitInfo{};
        submitInfo.setCommandBufferCount(1)
                .setPCommandBuffers(&commandBuffer);

        mDevice.getGraphicsQueue().submit(submitInfo);
        mDevice.getGraphicsQueue().waitIdle();

        mDevice.getDevice().freeCommandBuffers(mCommandPool, commandBuffer);
    }

    void VulkanCommandPool::submitOneTimeCommand(const std::function<void(const vk::CommandBuffer &)> &command) const {
        vk::CommandBuffer commandBuffer = allocateCommand();
        recordCommand(commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit, command);
        submitCommand(commandBuffer);
    }

    void VulkanCommandPool::recordCommand(const vk::CommandBuffer &commandBuffer, vk::CommandBufferUsageFlagBits usage, const std::function<void(const vk::CommandBuffer &)> &command) {
        vk::CommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo
                .setFlags(usage)
                .setPInheritanceInfo(nullptr);

        commandBuffer.begin(commandBufferBeginInfo);
        command(commandBuffer);
        commandBuffer.end();
    }
} // engine