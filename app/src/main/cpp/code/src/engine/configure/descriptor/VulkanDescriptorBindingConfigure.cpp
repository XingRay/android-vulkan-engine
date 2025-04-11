//
// Created by leixing on 2025/1/14.
//

#include "VulkanDescriptorBindingConfigure.h"

namespace engine {

    VulkanDescriptorBindingConfigure::VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange,
                                                                       vk::ShaderStageFlags shaderStageFlags,
                                                                       std::unique_ptr<VulkanDescriptorBufferInfoConfigure> &&vulkanDescriptorBufferInfoConfigure)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange), mShaderStageFlags(shaderStageFlags),
              mVulkanDescriptorBufferInfoConfigure(std::move(vulkanDescriptorBufferInfoConfigure)) {}

    VulkanDescriptorBindingConfigure::VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange,
                                                                       vk::ShaderStageFlags shaderStageFlags,
                                                                       std::unique_ptr<VulkanDescriptorImageInfoConfigure> &&vulkanDescriptorImageInfoConfigure)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange), mShaderStageFlags(shaderStageFlags),
              mVulkanDescriptorImageInfoConfigure(std::move(vulkanDescriptorImageInfoConfigure)) {}

    VulkanDescriptorBindingConfigure::~VulkanDescriptorBindingConfigure() = default;

    uint32_t VulkanDescriptorBindingConfigure::getBinding() const {
        return mBinding;
    }

    vk::DescriptorType VulkanDescriptorBindingConfigure::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t VulkanDescriptorBindingConfigure::getDescriptorOffset() const {
        return mDescriptorOffset;
    }

    uint32_t VulkanDescriptorBindingConfigure::getDescriptorRange() const {
        return mDescriptorRange;
    }

    vk::ShaderStageFlags VulkanDescriptorBindingConfigure::getShaderStageFlags() const {
        return mShaderStageFlags;
    }

    const std::vector<std::unique_ptr<VulkanSampler>> &VulkanDescriptorBindingConfigure::getImmutableSamplers() const {
        return mImmutableSamplers;
    }

    vk::DescriptorSetLayoutBinding VulkanDescriptorBindingConfigure::createDescriptorSetLayoutBinding() const {
        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

        descriptorSetLayoutBinding
                .setBinding(mBinding)
                .setDescriptorType(mDescriptorType)
                .setDescriptorCount(mDescriptorRange)
                .setStageFlags(mShaderStageFlags);

        if (!mImmutableSamplers.empty()) {
            std::vector<vk::Sampler> samplers;
            for (const std::unique_ptr<VulkanSampler> &vulkanSampler: mImmutableSamplers) {
                samplers.push_back(vulkanSampler->getSampler());
            }

            descriptorSetLayoutBinding
                    .setDescriptorCount(samplers.size())
                    .setPImmutableSamplers(samplers.data());
        }

        return descriptorSetLayoutBinding;
    }

    std::unique_ptr<VulkanBufferDescriptorBinding> VulkanDescriptorBindingConfigure::createVulkanBufferDescriptorBinding(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
        std::unique_ptr<VulkanBufferDescriptorBinding> vulkanBufferDescriptorBinding = std::make_unique<VulkanBufferDescriptorBinding>(mDescriptorType, mDescriptorOffset, mDescriptorRange);
        vulkanBufferDescriptorBinding->setBufferInfo(mVulkanDescriptorBufferInfoConfigure->provideVulkanDescriptorBufferInfo(vulkanDevice, commandPool));
        return vulkanBufferDescriptorBinding;
    }

    std::unique_ptr<VulkanImageDescriptorBinding> VulkanDescriptorBindingConfigure::createVulkanImageDescriptorBinding(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
        std::unique_ptr<VulkanImageDescriptorBinding> vulkanImageDescriptorBinding = std::make_unique<VulkanImageDescriptorBinding>(mDescriptorType, mDescriptorOffset, mDescriptorRange);
        vulkanImageDescriptorBinding->setVulkanDescriptorImageInfo(mVulkanDescriptorImageInfoConfigure->provideVulkanDescriptorImageInfo(vulkanDevice, commandPool));
        return vulkanImageDescriptorBinding;
    }
}