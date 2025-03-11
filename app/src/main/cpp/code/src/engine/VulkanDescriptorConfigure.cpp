//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanDescriptorConfigure.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareSampler.h"

namespace engine {

    VulkanDescriptorConfigure::VulkanDescriptorConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlagBits shaderStageFlagBits,
                                                         std::vector<std::unique_ptr<VulkanSampler>> &&immutableSamplers)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorCount(descriptorCount), mShaderStageFlagBits(shaderStageFlagBits), mImmutableSamplers(std::move(immutableSamplers)) {
    }

    VulkanDescriptorConfigure::VulkanDescriptorConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlagBits shaderStageFlagBits)
            : mBinding(binding), mDescriptorType(descriptorType), mDescriptorCount(descriptorCount), mShaderStageFlagBits(shaderStageFlagBits) {

    }

//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanUniformData data)
//            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {
//
//    }
//
//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanUniformData data)
//            : mVulkanDescriptorType(VulkanDescriptorType::uniform), mBinding(binding), mDescriptorType(vk::DescriptorType::eUniformBuffer), mShaderStageFlagBits(shaderStageFlagBits), mIndex(index),
//              mDescriptorCount(descriptorCount), mData(data) {
//
//    }
//
//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanSamplerData data)
//            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {
//
//    }
//
//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanSamplerData data)
//            : mVulkanDescriptorType(VulkanDescriptorType::sampler), mBinding(binding), mDescriptorType(vk::DescriptorType::eCombinedImageSampler), mShaderStageFlagBits(shaderStageFlagBits),
//              mIndex(index), mDescriptorCount(descriptorCount), mData(data) {
//
//    }
//
//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, VulkanAndroidHardwareBufferSamplerData data)
//            : VulkanDescriptor(binding, shaderStageFlagBits, 0, 1, data) {
//    }
//
//    VulkanDescriptor::VulkanDescriptor(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t index, uint32_t descriptorCount, VulkanAndroidHardwareBufferSamplerData data)
//            : mVulkanDescriptorType(VulkanDescriptorType::androidHardwareBufferSampler), mBinding(binding), mDescriptorType(vk::DescriptorType::eCombinedImageSampler),
//              mShaderStageFlagBits(shaderStageFlagBits), mIndex(index), mDescriptorCount(descriptorCount), mData(data) {
//    }

    VulkanDescriptorConfigure::~VulkanDescriptorConfigure() = default;

//    VulkanDescriptorType VulkanDescriptor::getVulkanDescriptorType() const {
//        return mVulkanDescriptorType;
//    }

    uint32_t VulkanDescriptorConfigure::getBinding() const {
        return mBinding;
    }

    vk::DescriptorType VulkanDescriptorConfigure::getDescriptorType() const {
        return mDescriptorType;
    }

//    uint32_t VulkanDescriptor::getIndex() const {
//        return mIndex;
//    }

    uint32_t VulkanDescriptorConfigure::getDescriptorCount() const {
        return mDescriptorCount;
    }

    vk::ShaderStageFlagBits VulkanDescriptorConfigure::getStageFlags() const {
        return mShaderStageFlagBits;
    }

    const std::vector<std::unique_ptr<VulkanSampler>> &VulkanDescriptorConfigure::getImmutableSamplers() const {
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

    vk::DescriptorSetLayoutBinding VulkanDescriptorConfigure::createDescriptorSetLayoutBinding() {
        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

        descriptorSetLayoutBinding
                .setBinding(mBinding)
                .setDescriptorType(mDescriptorType)
                .setDescriptorCount(mDescriptorCount)
                .setStageFlags(mShaderStageFlagBits);

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
}