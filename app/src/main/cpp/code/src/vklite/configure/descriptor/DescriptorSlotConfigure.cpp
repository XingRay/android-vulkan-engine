//
// Created by leixing on 2025/1/14.
//

#include "DescriptorSlotConfigure.h"

namespace vklite {

    DescriptorSlotConfigure::DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange,
                                                                       vk::ShaderStageFlags shaderStageFlags,
                                                                       std::unique_ptr<VulkanDescriptorBufferInfoConfigure> &&vulkanDescriptorBufferInfoConfigure)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange), mShaderStageFlags(shaderStageFlags),
              mVulkanDescriptorBufferInfoConfigure(std::move(vulkanDescriptorBufferInfoConfigure)) {}

    DescriptorSlotConfigure::DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange,
                                                                       vk::ShaderStageFlags shaderStageFlags,
                                                                       std::unique_ptr<VulkanDescriptorImageInfoConfigure> &&vulkanDescriptorImageInfoConfigure)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange), mShaderStageFlags(shaderStageFlags),
              mVulkanDescriptorImageInfoConfigure(std::move(vulkanDescriptorImageInfoConfigure)) {}

    DescriptorSlotConfigure::DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange,
                                                                       vk::ShaderStageFlags shaderStageFlags,
                                                                       std::unique_ptr<VulkanDescriptorImmutableSamplerConfigure> &&vulkanDescriptorImmutableSamplerConfigure)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange), mShaderStageFlags(shaderStageFlags),
              mVulkanDescriptorImmutableSamplerConfigure(std::move(vulkanDescriptorImmutableSamplerConfigure)) {}

    DescriptorSlotConfigure::~DescriptorSlotConfigure() = default;

    uint32_t DescriptorSlotConfigure::getBinding() const {
        return mBinding;
    }

    vk::DescriptorType DescriptorSlotConfigure::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t DescriptorSlotConfigure::getDescriptorOffset() const {
        return mDescriptorOffset;
    }

    uint32_t DescriptorSlotConfigure::getDescriptorRange() const {
        return mDescriptorRange;
    }

    vk::ShaderStageFlags DescriptorSlotConfigure::getShaderStageFlags() const {
        return mShaderStageFlags;
    }

//    const std::vector<std::unique_ptr<VulkanSamplerInterface>> &DescriptorSlotConfigure::getImmutableSamplers() const {
//        return mImmutableSamplers;
//    }

    vk::DescriptorSetLayoutBinding DescriptorSlotConfigure::createDescriptorSetLayoutBinding() const {
        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

        descriptorSetLayoutBinding
                .setBinding(mBinding)
                .setDescriptorType(mDescriptorType)
                .setDescriptorCount(mDescriptorRange)
                .setStageFlags(mShaderStageFlags);

        if (mVulkanDescriptorImmutableSamplerConfigure!= nullptr) {
            std::vector<vk::Sampler> samplers;
//            for (const std::unique_ptr<VulkanSamplerInterface> &vulkanSampler: mImmutableSamplers) {
//                samplers.push_back(vulkanSampler->getSampler());
//            }
//            mVulkanDescriptorImmutableSamplerConfigure
            descriptorSetLayoutBinding
                    .setDescriptorCount(samplers.size())
                    .setPImmutableSamplers(samplers.data());
        }

        return descriptorSetLayoutBinding;
    }

//    std::unique_ptr<VulkanDescriptorBinding> DescriptorSlotConfigure::createVulkanDescriptorBinding(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
//        std::unique_ptr<VulkanDescriptorBinding> vulkanDescriptorBinding = std::make_unique<VulkanDescriptorBinding>(mDescriptorType, mDescriptorOffset, mDescriptorRange);
//        if (mDescriptorType == vk::DescriptorType::eUniformBuffer) {
//            vulkanDescriptorBinding->setVulkanDescriptorBufferInfo(mVulkanDescriptorBufferInfoConfigure->provideVulkanDescriptorBufferInfo(vulkanDevice, commandPool));
//        } else if (mDescriptorType == vk::DescriptorType::eCombinedImageSampler) {
//            vulkanDescriptorBinding->setVulkanDescriptorImageInfo(mVulkanDescriptorImageInfoConfigure->provideVulkanDescriptorImageInfo(vulkanDevice, commandPool));
//        } else {
//            throw std::runtime_error("unknown descriptor type.");
//        }
//
//        return vulkanDescriptorBinding;
//    }
}