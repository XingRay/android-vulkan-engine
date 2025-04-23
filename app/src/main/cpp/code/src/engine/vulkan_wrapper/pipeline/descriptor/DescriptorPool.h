//
// Created by leixing on 2025/3/12.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    class DescriptorPool {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::DescriptorPool mDescriptorPool;
    public:
        DescriptorPool(const VulkanDevice &vulkanDevice, std::vector<vk::DescriptorPoolSize> descriptorPoolSizes, uint32_t maxSets);

        ~DescriptorPool();

        [[nodiscard]]
        const vk::DescriptorPool &getDescriptorPool() const;

        std::vector<vk::DescriptorSet> allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts);
    };

} // engine
