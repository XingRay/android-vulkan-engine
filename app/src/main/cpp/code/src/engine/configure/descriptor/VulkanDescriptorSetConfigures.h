//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "VulkanDescriptorSetConfigure.h"
#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBindingSets.h"

namespace engine {

    class VulkanDescriptorSetConfigures {
    private:
        // set -> setConfigure
        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorSetConfigure>> mVulkanDescriptorSetConfigures;

    public:
        VulkanDescriptorSetConfigures();

        ~VulkanDescriptorSetConfigures();

        VulkanDescriptorSetConfigures &addVulkanDescriptorSetConfigure(std::unique_ptr<VulkanDescriptorSetConfigure> &&vulkanDescriptorSetConfigure);

        [[nodiscard]]
        std::vector<vk::DescriptorSetLayout> createDescriptorSetLayouts(const VulkanDevice &vulkanDevice) const;

        [[nodiscard]]
        std::vector<vk::DescriptorPoolSize> createDescriptorPoolSizes(uint32_t frameCount = 1) const;

        [[nodiscard]]
        uint32_t getSetCount(uint32_t frameCount = 1) const;

        [[nodiscard]]
        std::unique_ptr<VulkanDescriptorBindingSets> createVulkanDescriptorBindingSets(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
