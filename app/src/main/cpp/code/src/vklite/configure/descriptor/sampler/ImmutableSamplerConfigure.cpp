//
// Created by leixing on 2025/3/24.
//

#include "ImmutableSamplerConfigure.h"
#include "vklite/configure/descriptor/DescriptorSlotConfigure.h"

namespace vklite {

    ImmutableSamplerConfigure::ImmutableSamplerConfigure()
            : mBinding(0), mDescriptorOffset(0), mDescriptorRange(1) {}

    ImmutableSamplerConfigure::~ImmutableSamplerConfigure() = default;

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::descriptorOffset(uint32_t offset) {
        mDescriptorOffset = offset;
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::descriptorRange(uint32_t range) {
        mDescriptorRange = range;
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

//    std::unique_ptr<VulkanDescriptorBindingConfigure> ImmutableSamplerConfigure::createVulkanDescriptorBindingConfigure() {
//        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eCombinedImageSampler, mDescriptorOffset, mDescriptorRange, mShaderStageFlags,
//                                                                  std::move(mVulkanDescriptorImmutableSamplerConfigure));
//    }

} // engine