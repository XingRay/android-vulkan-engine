//
// Created by leixing on 2025/1/14.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanUniform {
    private:
        uint32_t set;
        uint32_t binding;

    public:

        virtual ~VulkanUniform() = default;

        virtual vk::DescriptorType getDescriptorType() = 0;
    };

    class VulkanNormalUniform : public VulkanUniform {

    public:
        vk::DescriptorType getDescriptorType() override;

    };

    class VulkanSamplerUniform : public VulkanUniform {

    public:
        vk::DescriptorType getDescriptorType() override;

    };

} // engine
