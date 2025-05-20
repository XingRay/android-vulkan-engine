//
// Created by leixing on 2025/3/28.
//

#include "Descriptor.h"

namespace vklite {

    Descriptor::Descriptor(vk::DescriptorType descriptorType,
                           uint32_t descriptorCount,
                           vk::ShaderStageFlags shaderStageFlags)
            : mDescriptorType(descriptorType),
              mDescriptorCount(descriptorCount),
              mShaderStageFlags(shaderStageFlags),
              mImmutableSamplers(),
              mImmutableVkSamplers() {

    }

    Descriptor::Descriptor(vk::DescriptorType descriptorType,
                           std::vector<std::unique_ptr<SamplerInterface>> &&immutableSamplers,
                           vk::ShaderStageFlags shaderStageFlags)
            : mDescriptorType(descriptorType),
              mImmutableSamplers(std::move(immutableSamplers)),
              mShaderStageFlags(shaderStageFlags) {

        mDescriptorCount = mImmutableSamplers.size();
        mImmutableVkSamplers.reserve(mImmutableSamplers.size());

        for (const std::unique_ptr<SamplerInterface> &sampler: mImmutableSamplers) {
            mImmutableVkSamplers.push_back(sampler->getSampler());
        }
    }

    Descriptor::Descriptor(Descriptor &&other) noexcept
            : mDescriptorType(other.mDescriptorType),
              mDescriptorCount(other.mDescriptorCount),
              mShaderStageFlags(other.mShaderStageFlags),
              mImmutableSamplers(std::move(other.mImmutableSamplers)),
              mImmutableVkSamplers(std::move(other.mImmutableVkSamplers)) {}

    Descriptor::~Descriptor() = default;

    Descriptor &Descriptor::operator=(Descriptor &&other) noexcept {
        if (this != &other) {
            mDescriptorType = other.mDescriptorType;
            mDescriptorCount = other.mDescriptorCount;
            mShaderStageFlags = other.mShaderStageFlags;
            mImmutableSamplers = std::move(other.mImmutableSamplers);
            mImmutableVkSamplers = std::move(other.mImmutableVkSamplers);
        }
        return *this;
    }

    vk::DescriptorType Descriptor::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t Descriptor::getDescriptorCount() const {
        return mDescriptorCount;
    }

    vk::ShaderStageFlags Descriptor::getShaderStageFlags() const {
        return mShaderStageFlags;
    }

    const std::vector<vk::Sampler> &Descriptor::getImmutableVkSamplers() const {
        return mImmutableVkSamplers;
    }

//    const std::unique_ptr<VulkanDescriptorBufferInfo> &Descriptor::getVulkanDescriptorBufferInfo() const {
//        return mVulkanDescriptorBufferInfo;
//    }
//
//    Descriptor &Descriptor::setVulkanDescriptorBufferInfo(std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanDescriptorBufferInfo) {
//        mVulkanDescriptorBufferInfo = std::move(vulkanDescriptorBufferInfo);
//        return *this;
//    }
//
//    const std::unique_ptr<VulkanDescriptorImageInfo> &Descriptor::getVulkanDescriptorImageInfo() const {
//        return mVulkanDescriptorImageInfo;
//    }
//
//    Descriptor &Descriptor::setVulkanDescriptorImageInfo(std::unique_ptr<VulkanDescriptorImageInfo> &&vulkanDescriptorImageInfo) {
//        mVulkanDescriptorImageInfo = std::move(vulkanDescriptorImageInfo);
//        return *this;
//    }
//
//    Descriptor &Descriptor::updateUniformBuffer(const VulkanCommandPool &commandPool, const void *data, uint32_t size) {
//        mVulkanDescriptorBufferInfo->update(commandPool, data, size);
//        return *this;
//    }

//    vk::DescriptorSetLayoutBinding Descriptor::createDescriptorSetLayoutBinding(uint32_t binding) const {
//        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};
//
//        descriptorSetLayoutBinding
//                .setBinding(binding)
//                .setDescriptorType(mDescriptorType)
//                .setDescriptorCount(mDescriptorCount)
//                .setStageFlags(mShaderStageFlags);
//
//        if (mImmutableSamplerInfo != nullptr) {
//            std::vector<vk::Sampler> samplers;
////            for (const std::unique_ptr<VulkanSamplerInterface> &vulkanSampler: mImmutableSamplers) {
////                samplers.push_back(vulkanSampler->getSampler());
////            }
//            // todo: sampler array
//            samplers.push_back(mImmutableSamplerInfo->getVulkanSampler()->getSampler());
//            descriptorSetLayoutBinding
//                    .setDescriptorCount(samplers.size())
//                    .setPImmutableSamplers(samplers.data());
//        }
//
//        return descriptorSetLayoutBinding;
//    }

} // engine