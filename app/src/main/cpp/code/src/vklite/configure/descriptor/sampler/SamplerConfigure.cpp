//
// Created by leixing on 2025/3/24.
//

#include "SamplerConfigure.h"
#include "vklite/configure/descriptor/DescriptorSlotConfigure.h"

#include "vklite/image/VulkanImageBuilder.h"
#include "vklite/sampler/builder/DefaultSamplerBuilder.h"

namespace vklite {

    SamplerConfigure::SamplerConfigure()
            : mBinding(0), mDescriptorOffset(0), mDescriptorRange(1) {}

    SamplerConfigure::~SamplerConfigure() = default;

    SamplerConfigure &SamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::descriptorOffset(uint32_t offset) {
        mDescriptorOffset = offset;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::descriptorRange(uint32_t range) {
        mDescriptorRange = range;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::setImage(std::unique_ptr<vklite::ImageInterface> &&image) {
        mVulkanDescriptorImageInfoConfigure = std::make_unique<VulkanDescriptorImageInfoConfigure>(
                std::make_unique<VulkanImageBuilder>(),
                std::make_unique<DefaultSamplerBuilder>(),
                std::move(image)
        );
        return *this;
    }

//    std::unique_ptr<VulkanDescriptorBindingConfigure> SamplerConfigure::createVulkanDescriptorBindingConfigure() {
//        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eCombinedImageSampler, mDescriptorOffset, mDescriptorRange, mShaderStageFlags,
//                                                                  std::move(mVulkanDescriptorImageInfoConfigure));
//    }

} // engine