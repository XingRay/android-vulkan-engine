//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <vector>

#include "engine/VulkanDescriptorSetConfigure.h"

namespace engine {

    class VulkanDescriptorSetConfigures {
    private:
        std::vector<std::unique_ptr<VulkanDescriptorSetConfigure>> mVulkanDescriptorSetConfigures;

    public:
        VulkanDescriptorSetConfigures();

        ~VulkanDescriptorSetConfigures();

        VulkanDescriptorSetConfigures& addVulkanDescriptorSetConfigure(std::unique_ptr<VulkanDescriptorSetConfigure>&& vulkanDescriptorSetConfigure);

        std::vector<vk::DescriptorSetLayout> createDescriptorSetLayouts(const VulkanDevice& vulkanDevice);

        std::vector<vk::DescriptorPoolSize> createDescriptorPoolSizes();
    };

} // engine
