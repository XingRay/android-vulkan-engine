//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanUniform.h"

namespace engine {

    /**
     *
     * VulkanUniform
     *
     */
    VulkanUniform::VulkanUniform(uint32_t binding, vk::DescriptorType descriptorType)
            : mBinding(binding), mDescriptorType(descriptorType) {

    }

    VulkanUniform::~VulkanUniform() = default;

    vk::DescriptorType VulkanUniform::getDescriptorType() {
        return mDescriptorType;
    }


    /**
     *
     * VulkanNormalUniform
     *
     */
    VulkanNormalUniform::VulkanNormalUniform(uint32_t binding)
            : VulkanUniform{binding, vk::DescriptorType::eUniformBuffer} {

    }

    VulkanNormalUniform::~VulkanNormalUniform() {

    }


    /**
     *
     * VulkanSamplerUniform
     *
     */
    VulkanSamplerUniform::VulkanSamplerUniform(uint32_t binding)
            : VulkanUniform{binding, vk::DescriptorType::eCombinedImageSampler} {

    }

    VulkanSamplerUniform::~VulkanSamplerUniform() = default;
}