//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <memory>
#include "engine/VulkanDescriptorConfigure.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    class VulkanDescriptorSetConfigure {
    private:
        uint32_t mSet;
        std::vector<std::unique_ptr<VulkanDescriptorConfigure>> mVulkanDescriptorConfigures;

    public:

        VulkanDescriptorSetConfigure();

        ~VulkanDescriptorSetConfigure();

        uint32_t getSet()const;

        const std::vector<std::unique_ptr<VulkanDescriptorConfigure>>& getVulkanDescriptorConfigures();

        VulkanDescriptorSetConfigure &set(uint32_t set);

        VulkanDescriptorSetConfigure &addVulkanDescriptor(std::unique_ptr<VulkanDescriptorConfigure> &&vulkanDescriptor);

        VulkanDescriptorSetConfigure &addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        VulkanDescriptorSetConfigure &addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        VulkanDescriptorSetConfigure &addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        VulkanDescriptorSetConfigure &addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings();

        vk::DescriptorSetLayout createDescriptorSetLayout(const VulkanDevice& vulkanDevice);
    };

} // engine
