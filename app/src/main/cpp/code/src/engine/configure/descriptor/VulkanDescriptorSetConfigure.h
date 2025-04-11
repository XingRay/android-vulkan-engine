//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "VulkanDescriptorBindingConfigure.h"
#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBindingSet.h"
#include "engine/configure/uniform/VulkanUniformConfigure.h"
#include "engine/configure/sampler/VulkanSamplerConfigure.h"

namespace engine {

    class VulkanDescriptorSetConfigure {
    private:
        uint32_t mSet;
        // binding -> BindingConfigure
        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> mVulkanDescriptorBindingConfigures;

    public:

        VulkanDescriptorSetConfigure();

        ~VulkanDescriptorSetConfigure();

        uint32_t getSet() const;

        const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> &getVulkanDescriptorBindingConfigures();

        VulkanDescriptorSetConfigure &set(uint32_t set);

        //todo [functional configure]
//        VulkanDescriptorSetConfigure &addVulkanDescriptor(std::unique_ptr<VulkanDescriptorBindingConfigure> &&vulkanDescriptor);

//        VulkanDescriptorSetConfigure &addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        VulkanDescriptorSetConfigure &addUniform(const std::function<void(VulkanUniformConfigure &)> &configure);

//        VulkanDescriptorSetConfigure &addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        VulkanDescriptorSetConfigure &addSampler(const std::function<void(VulkanSamplerConfigure &)> &configure);
//
//        VulkanDescriptorSetConfigure &addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);
//
//        VulkanDescriptorSetConfigure &addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        std::unique_ptr<VulkanDescriptorBindingSet> createVulkanDescriptorBindingSet(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);

    private:
        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings();

        vk::DescriptorSetLayout createDescriptorSetLayout(const VulkanDevice &vulkanDevice);
    };

} // engine
