//
// Created by leixing on 2025/3/24.
//

#include "engine/VulkanSamplerConfigure.h"

namespace engine {

    VulkanSamplerConfigure::VulkanSamplerConfigure() = default;

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
        mVulkanDescriptorImageInfoConfigure = std::make_unique<VulkanDescriptorImageInfoConfigure>(std::move(image));
        return *this;
    }

    std::unique_ptr<VulkanDescriptorBindingConfigure> VulkanSamplerConfigure::createVulkanDescriptorBindingConfigure() {
//        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eUniformBuffer, mDescriptorOffset, mDescriptorRange, mShaderStageFlags,
//                                                                  std::move(mVulkanBufferViewConfigure));
        return nullptr;
    }

} // engine