//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/ImageInterface.h"
#include "vklite/configure/descriptor/VulkanDescriptorImageInfoConfigure.h"

namespace vklite {

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

        SamplerConfigure &setImage(std::unique_ptr<vklite::ImageInterface> &&image);

    private:

//        [[nodiscard]]
//        std::unique_ptr<VulkanDescriptorBindingConfigure> createVulkanDescriptorBindingConfigure();
    };

} // engine
