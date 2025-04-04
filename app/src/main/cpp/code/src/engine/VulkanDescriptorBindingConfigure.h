//
// Created by leixing on 2025/1/14.
//

#pragma once

//#include <variant>
#include <memory>
#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/buffer/VulkanSampler.h"
#include "engine/vulkan_wrapper/VulkanBufferDescriptorBinding.h"
#include "engine/vulkan_wrapper/VulkanImageDescriptorBinding.h"
#include "engine/VulkanBufferViewConfigure.h"

namespace engine {

    class VulkanDescriptorBindingConfigure {
    private:
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;
        std::vector<std::unique_ptr<VulkanSampler>> mImmutableSamplers;

        std::unique_ptr<VulkanBufferViewConfigure> mVulkanBufferViewConfigure;

    public:

//        VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags,
//                                         std::vector<std::unique_ptr<VulkanSampler>> &&immutableSamplers);

        VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<VulkanBufferViewConfigure> &&vulkanBufferViewConfigure);

//        VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags,
//                                         std::unique_ptr<VulkanImageViewConfigure> &&VulkanBufferViewConfigure);

        ~VulkanDescriptorBindingConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorOffset() const;

        [[nodiscard]]
        uint32_t getDescriptorRange() const;

        [[nodiscard]]
        vk::ShaderStageFlags getShaderStageFlags() const;

        [[nodiscard]]
        const std::vector<std::unique_ptr<VulkanSampler>> &getImmutableSamplers() const;

        [[nodiscard]]
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding() const;

        [[nodiscard]]
        std::unique_ptr<VulkanBufferDescriptorBinding> createVulkanBufferDescriptorBinding() const;
    };

} // engine
