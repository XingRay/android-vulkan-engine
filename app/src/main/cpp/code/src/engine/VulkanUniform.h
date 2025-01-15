//
// Created by leixing on 2025/1/14.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanUniform {
    protected:
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;

    public:

        VulkanUniform(uint32_t binding, vk::DescriptorType descriptorType);

        virtual ~VulkanUniform();

        vk::DescriptorType getDescriptorType();
    };

    class VulkanNormalUniform : public VulkanUniform {

    public:
        VulkanNormalUniform(uint32_t binding);

        ~VulkanNormalUniform();
    };

    class VulkanSamplerUniform : public VulkanUniform {

    public:

        VulkanSamplerUniform(uint32_t binding);

        ~VulkanSamplerUniform();
    };

} // engine
