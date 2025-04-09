//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "engine/ImageInterface.h"
#include "engine/VulkanDescriptorImageInfoConfigure.h"

namespace engine {

    class VulkanDescriptorBindingConfigure;

    class VulkanDescriptorSetConfigure;

    class VulkanSamplerConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<VulkanDescriptorImageInfoConfigure> mVulkanDescriptorImageInfoConfigure;

    public:
        VulkanSamplerConfigure();

        ~VulkanSamplerConfigure();

        VulkanSamplerConfigure &binding(uint32_t binding);

        VulkanSamplerConfigure &descriptorOffset(uint32_t offset);

        VulkanSamplerConfigure &descriptorRange(uint32_t range);

        VulkanSamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        VulkanSamplerConfigure &setImage(std::unique_ptr<engine::ImageInterface> &&image);

    private:

        [[nodiscard]]
        std::unique_ptr<VulkanDescriptorBindingConfigure> createVulkanDescriptorBindingConfigure();
    };

} // engine
