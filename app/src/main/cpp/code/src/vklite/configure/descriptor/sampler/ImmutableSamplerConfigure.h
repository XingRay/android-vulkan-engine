//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/sampler/builder/SamplerBuilderInterface.h"
#include "vklite/configure/descriptor/VulkanDescriptorImmutableSamplerConfigure.h"

namespace vklite {

//    class VulkanDescriptorBindingConfigure;

//    class VulkanDescriptorSetConfigure;

    class ImmutableSamplerConfigure {
//        friend VulkanDescriptorSetConfigure;

    private:
        uint32_t mBinding;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<VulkanDescriptorImmutableSamplerConfigure> mVulkanDescriptorImmutableSamplerConfigure;

    public:
        ImmutableSamplerConfigure();

        ~ImmutableSamplerConfigure();

        ImmutableSamplerConfigure &binding(uint32_t binding);

        ImmutableSamplerConfigure &descriptorOffset(uint32_t offset);

        ImmutableSamplerConfigure &descriptorRange(uint32_t range);

        ImmutableSamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

//        std::unique_ptr<VulkanDescriptorBindingConfigure> createVulkanDescriptorBindingConfigure();
    };

} // engine
