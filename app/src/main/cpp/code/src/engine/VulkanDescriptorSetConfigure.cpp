//
// Created by leixing on 2025/1/15.
//

#include "VulkanDescriptorSetConfigure.h"

namespace engine {

    VulkanDescriptorSetConfigure::VulkanDescriptorSetConfigure() {

    }

    VulkanDescriptorSetConfigure::~VulkanDescriptorSetConfigure() = default;

    uint32_t VulkanDescriptorSetConfigure::getSet() const {
        return mSet;
    }

    const std::vector<std::unique_ptr<VulkanDescriptorConfigure>> &VulkanDescriptorSetConfigure::getVulkanDescriptorConfigures() {
        return mVulkanDescriptorConfigures;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::set(uint32_t set) {
        mSet = set;
        return *this;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addVulkanDescriptor(std::unique_ptr<VulkanDescriptorConfigure> &&vulkanDescriptor) {
        mVulkanDescriptorConfigures.push_back(std::move(vulkanDescriptor));
        return *this;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
        mVulkanDescriptorConfigures.push_back(std::make_unique<VulkanDescriptorConfigure>(binding, vk::DescriptorType::eUniformBuffer, descriptorCount, shaderStageFlagBits));
        return *this;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addUniform(const std::function<void(VulkanUniformConfigure &)> &configure) {
        VulkanUniformConfigure config{};
        configure(config);
        mVulkanUniformConfigures.push_back(config);
//        mVulkanDescriptorConfigures.push_back(std::make_unique<VulkanDescriptorConfigure>(binding, vk::DescriptorType::eUniformBuffer, descriptorCount, shaderStageFlagBits));
        return *this;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
        mVulkanDescriptorConfigures.push_back(std::make_unique<VulkanDescriptorConfigure>(binding, vk::DescriptorType::eCombinedImageSampler, descriptorCount, shaderStageFlagBits));
        return *this;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
        mVulkanDescriptorConfigures.push_back(std::make_unique<VulkanDescriptorConfigure>(binding, vk::DescriptorType::eCombinedImageSampler, descriptorCount, shaderStageFlagBits));
        return *this;
    }

    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
        mVulkanDescriptorConfigures.push_back(std::make_unique<VulkanDescriptorConfigure>(binding, vk::DescriptorType::eStorageBuffer, descriptorCount, shaderStageFlagBits));
        return *this;
    }


//    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t size) {
//        VulkanUniformData data{size};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, ImageSize imageSize) {
//        VulkanSamplerData data{imageSize};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    VulkanDescriptorSetConfigure &VulkanDescriptorSetConfigure::addAndroidHardwareBufferSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, AHardwareBuffer *hardwareBuffer) {
//        VulkanAndroidHardwareBufferSamplerData data{hardwareBuffer};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    const VulkanDescriptorSet &VulkanDescriptorSetConfigure::build() const {
//        return mUniformSet;
//    }

    std::vector<vk::DescriptorSetLayoutBinding> VulkanDescriptorSetConfigure::createDescriptorSetLayoutBindings() {

        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
        descriptorSetLayoutBindings.reserve(mVulkanDescriptorConfigures.size());

        for (const std::unique_ptr<VulkanDescriptorConfigure> &vulkanDescriptorConfigure: mVulkanDescriptorConfigures) {
            vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding = vulkanDescriptorConfigure->createDescriptorSetLayoutBinding();
            descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
        }

        for (const VulkanUniformConfigure &vulkanUniformConfigure: mVulkanUniformConfigures) {
            vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding = vulkanUniformConfigure.createDescriptorSetLayoutBinding();
            descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
        }

        return descriptorSetLayoutBindings;
    }

    vk::DescriptorSetLayout VulkanDescriptorSetConfigure::createDescriptorSetLayout(const VulkanDevice &vulkanDevice) {

        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = createDescriptorSetLayoutBindings();

        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
        descriptorSetLayoutCreateInfo
                .setBindings(descriptorSetLayoutBindings);

        return vulkanDevice.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
    }

} // engine