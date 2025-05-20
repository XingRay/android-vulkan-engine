//
// Created by leixing on 2025/1/14.
//

#pragma once

//#include <variant>
#include <memory>
#include "vulkan/vulkan.hpp"

#include "vklite/sampler/SamplerInterface.h"

#include "vklite/pipeline/descriptor/Descriptor.h"
#include "vklite/configure/descriptor/VulkanDescriptorBufferInfoConfigure.h"
#include "vklite/buffer/VulkanUniformBufferBuilder.h"
#include "vklite/configure/descriptor/VulkanDescriptorImageInfoConfigure.h"
#include "vklite/configure/descriptor/VulkanDescriptorImmutableSamplerConfigure.h"

namespace vklite {

    class DescriptorSlotConfigure {
    private:
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<VulkanDescriptorImmutableSamplerConfigure> mVulkanDescriptorImmutableSamplerConfigure;
        std::unique_ptr<VulkanDescriptorBufferInfoConfigure> mVulkanDescriptorBufferInfoConfigure;
        std::unique_ptr<VulkanDescriptorImageInfoConfigure> mVulkanDescriptorImageInfoConfigure;

    public:

        DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<VulkanDescriptorBufferInfoConfigure> &&vulkanDescriptorBufferInfoConfigure);

        DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<VulkanDescriptorImageInfoConfigure> &&vulkanDescriptorImageInfoConfigure);

        DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<VulkanDescriptorImmutableSamplerConfigure> &&vulkanDescriptorImmutableSamplerConfigure);

        ~DescriptorSlotConfigure();

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
        const std::vector<std::unique_ptr<SamplerInterface>> &getImmutableSamplers() const;

        [[nodiscard]]
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding() const;

//        [[nodiscard]]
//        std::unique_ptr<VulkanDescriptorBinding> createVulkanDescriptorBinding(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);
    };

} // engine
