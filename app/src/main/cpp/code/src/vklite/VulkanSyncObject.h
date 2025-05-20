//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/Log.h"

namespace vklite {

    class VulkanSyncObject {
    private:
        const VulkanDevice &mDevice;

        //vk::Semaphore 主要用于 GPU 之间的同步
        std::vector<vk::Semaphore> mImageAvailableSemaphores;
        std::vector<vk::Semaphore> mRenderFinishedSemaphores;

        // vk::Fence 主要用于 CPU 和 GPU 之间的同步，例如：
        // 等待 GPU 完成某个操作, 确保 CPU 不会过早地访问 GPU 资源。
        std::vector<vk::Fence> mFences;

    public:
        explicit VulkanSyncObject(const VulkanDevice &vulkanDevice, uint32_t count);

        ~VulkanSyncObject();

        [[nodiscard]]
        const vk::Semaphore &getImageAvailableSemaphore(uint32_t index) const;

        [[nodiscard]]
        const vk::Semaphore &getRenderFinishedSemaphore(uint32_t index) const;

        [[nodiscard]]
        vk::Fence getFence(uint32_t index) const;

        vk::Result resetFence(uint32_t index);

        vk::Result waitFence(uint32_t index);
    };

} // engine
