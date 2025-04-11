//
// Created by leixing on 2025/3/24.
//

#include "engine/configure/sampler/VulkanSamplerConfigure.h"
#include "engine/configure/descriptor/VulkanDescriptorBindingConfigure.h"

#include "engine/vulkan_wrapper/image/VulkanImageBuilder.h"
#include "engine/vulkan_wrapper/image/VulkanSamplerBuilder.h"

namespace engine {

    VulkanSamplerConfigure::VulkanSamplerConfigure()
            : mBinding(0), mDescriptorOffset(0), mDescriptorRange(1) {}

    VulkanSamplerConfigure::~VulkanSamplerConfigure() = default;

    VulkanSamplerConfigure &VulkanSamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VulkanSamplerConfigure &VulkanSamplerConfigure::descriptorOffset(uint32_t offset) {
        mDescriptorOffset = offset;
        return *this;
    }

    VulkanSamplerConfigure &VulkanSamplerConfigure::descriptorRange(uint32_t range) {
        mDescriptorRange = range;
        return *this;
    }

    VulkanSamplerConfigure &VulkanSamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    VulkanSamplerConfigure &VulkanSamplerConfigure::setImage(std::unique_ptr<engine::ImageInterface> &&image) {
        mVulkanDescriptorImageInfoConfigure = std::make_unique<VulkanDescriptorImageInfoConfigure>(
                std::make_unique<VulkanImageBuilder>(),
                std::make_unique<VulkanSamplerBuilder>(),
                std::move(image)
        );
        return *this;
    }

    std::unique_ptr<VulkanDescriptorBindingConfigure> VulkanSamplerConfigure::createVulkanDescriptorBindingConfigure() {
        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eCombinedImageSampler, mDescriptorOffset, mDescriptorRange, mShaderStageFlags,
                                                                  std::move(mVulkanDescriptorImageInfoConfigure));
    }

} // engine