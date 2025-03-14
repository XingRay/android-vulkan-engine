//
// Created by leixing on 2025/1/14.
//

#pragma once

//#include <variant>
#include <memory>
#include "vulkan/vulkan.hpp"

//#include "engine/ImageSize.h"

#include "engine/vulkan_wrapper/buffer/VulkanSampler.h"

namespace engine {

    class VulkanDescriptorConfigure {
    private:
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlagBits mShaderStageFlagBits;
        std::vector<std::unique_ptr<VulkanSampler>> mImmutableSamplers;

    public:

        VulkanDescriptorConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlagBits shaderStageFlagBits,
                                  std::vector<std::unique_ptr<VulkanSampler>> &&immutableSamplers);

        VulkanDescriptorConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlagBits shaderStageFlagBits);

        ~VulkanDescriptorConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorCount() const;

        [[nodiscard]]
        vk::ShaderStageFlagBits getStageFlags() const;

        [[nodiscard]]
        const std::vector<std::unique_ptr<VulkanSampler>> &getImmutableSamplers() const;

        [[nodiscard]]
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding();
    };

} // engine
