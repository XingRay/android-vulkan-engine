//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "engine/ImageInterface.h"
#include "engine/configure/descriptor/VulkanDescriptorImageInfoConfigure.h"

namespace engine {

//    class VulkanDescriptorBindingConfigure;

//    class VulkanDescriptorSetConfigure;

    class SamplerConfigure {
//        friend VulkanDescriptorSetConfigure;

    private:
        uint32_t mBinding;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<VulkanDescriptorImageInfoConfigure> mVulkanDescriptorImageInfoConfigure;

    public:
        SamplerConfigure();

        ~SamplerConfigure();

        SamplerConfigure &binding(uint32_t binding);

        SamplerConfigure &descriptorOffset(uint32_t offset);

        SamplerConfigure &descriptorRange(uint32_t range);

        SamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        SamplerConfigure &setImage(std::unique_ptr<engine::ImageInterface> &&image);

    private:

//        [[nodiscard]]
//        std::unique_ptr<VulkanDescriptorBindingConfigure> createVulkanDescriptorBindingConfigure();
    };

} // engine
