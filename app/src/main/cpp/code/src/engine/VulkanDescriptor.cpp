//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanDescriptor.h"

namespace engine {

    /**
     *
     * VulkanUniform
     *
     */
    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanUniformData data)
            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {

    }

    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanUniformData data)
            : mVulkanDescriptorType(VulkanDescriptorType::uniform), mBinding(binding), mDescriptorType(vk::DescriptorType::eUniformBuffer), mShaderStageFlagBits(shaderStageFlagBits), mIndex(index),
              mDescriptorCount(descriptorCount), mData(data) {

    }

    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanSamplerData data)
            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {

    }

    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanSamplerData data)
            : mVulkanDescriptorType(VulkanDescriptorType::sampler), mBinding(binding), mDescriptorType(vk::DescriptorType::eCombinedImageSampler), mShaderStageFlagBits(shaderStageFlagBits),
              mIndex(index), mDescriptorCount(descriptorCount), mData(data) {

    }

    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanAndroidHardwareBufferSamplerData data)
            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {
    }

    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanAndroidHardwareBufferSamplerData data)
            : mVulkanDescriptorType(VulkanDescriptorType::androidHardwareBufferSampler), mBinding(binding), mDescriptorType(vk::DescriptorType::eCombinedImageSampler),
              mShaderStageFlagBits(shaderStageFlagBits), mIndex(index), mDescriptorCount(descriptorCount), mData(data) {
    }

    VulkanDescriptor::~VulkanDescriptor() = default;

    VulkanDescriptorType VulkanDescriptor::getVulkanDescriptorType() const {
        return mVulkanDescriptorType;
    }

    uint32_t VulkanDescriptor::getBinding() const {
        return mBinding;
    }

    vk::DescriptorType VulkanDescriptor::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t VulkanDescriptor::getIndex() const {
        return mIndex;
    }

    uint32_t VulkanDescriptor::getDescriptorCount() const {
        return mDescriptorCount;
    }

    vk::ShaderStageFlagBits VulkanDescriptor::getStageFlags() const {
        return mShaderStageFlagBits;
    }

    const std::vector<VulkanSampler> &VulkanDescriptor::getImmutableSamplers()const {
        return mImmutableSamplers;
    }

    const VulkanUniformData &VulkanDescriptor::getUniformData() const {
        return std::get<VulkanUniformData>(mData);
    }

    const VulkanSamplerData &VulkanDescriptor::getSamplerData() const {
        return std::get<VulkanSamplerData>(mData);
    }

    const VulkanAndroidHardwareBufferSamplerData &VulkanDescriptor::getVulkanAndroidHardwareBufferSamplerData() const {
        return std::get<VulkanAndroidHardwareBufferSamplerData>(mData);
    }
}