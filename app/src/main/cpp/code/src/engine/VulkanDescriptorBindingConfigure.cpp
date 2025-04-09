//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanDescriptorBindingConfigure.h"

namespace engine {

//    VulkanDescriptorBindingConfigure::VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags,
//                                                                       std::vector<std::unique_ptr<VulkanSampler>> &&immutableSamplers)
//            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags), mImmutableSamplers(std::move(immutableSamplers)) {}

    VulkanDescriptorBindingConfigure::VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange,
                                                                       vk::ShaderStageFlags shaderStageFlags,
                                                                       std::unique_ptr<VulkanDescriptorBufferInfoConfigure> &&vulkanDescriptorBufferInfoConfigure)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange), mShaderStageFlags(shaderStageFlags),
              mVulkanDescriptorBufferInfoConfigure(std::move(vulkanDescriptorBufferInfoConfigure)) {}

//    VulkanDescriptorBindingConfigure::VulkanDescriptorBindingConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags,
//                                                                       std::unique_ptr<VulkanImageView> &&vulkanImageView)
//            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags), mVulkanImageView(std::move(vulkanImageView)) {}

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanUniformData data)
//            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {}

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanUniformData data)
//            : mVulkanDescriptorType(VulkanDescriptorType::uniform), mBinding(binding), mDescriptorType(vk::DescriptorType::eUniformBuffer), mShaderStageFlagBits(shaderStageFlagBits), mIndex(index),
//              mDescriptorCount(descri       ptorCount), mData(data) {}

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanSamplerData data)
//            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {}

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanSamplerData data)
//            : mVulkanDescriptorType(VulkanDescriptorType::sampler), mBinding(binding), mDescriptorType(vk::DescriptorType::eCombinedImageSampler), mShaderStageFlagBits(shaderStageFlagBits),
//              mIndex(index), mDescriptorCount(descriptorCount), mData(data) {}

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanAndroidHardwareBufferSamplerData data)
//            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {}

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanAndroidHardwareBufferSamplerData data)
//            : mVulkanDescriptorType(VulkanDescriptorType::androidHardwareBufferSampler), mBinding(binding), mDescriptorType(vk::DescriptorType::eCombinedImageSampler),
//              mShaderStageFlagBits(shaderStageFlagBits), mIndex(index), mDescriptorCount(descriptorCount), mData(data) {}

    VulkanDescriptorBindingConfigure::~VulkanDescriptorBindingConfigure() = default;

//    VulkanDescriptorType VulkanDescriptor::getVulkanDescriptorType() const {
//        return mVulkanDescriptorType;
//    }

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

//    const VulkanUniformData &VulkanDescriptor::getUniformData() const {
//        return std::get<VulkanUniformData>(mData);
//    }
//
//    const VulkanSamplerData &VulkanDescriptor::getSamplerData() const {
//        return std::get<VulkanSamplerData>(mData);
//    }
//
//    const VulkanAndroidHardwareBufferSamplerData &VulkanDescriptor::getVulkanAndroidHardwareBufferSamplerData() const {
//        return std::get<VulkanAndroidHardwareBufferSamplerData>(mData);
//    }

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
}