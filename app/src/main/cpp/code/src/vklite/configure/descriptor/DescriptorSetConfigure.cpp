//
// Created by leixing on 2025/1/15.
//

#include "DescriptorSetConfigure.h"
#include "vklite/buffer/VulkanUniformBufferBuilder.h"

namespace vklite {

    DescriptorSetConfigure::DescriptorSetConfigure() : mSet(0) {

    }

    DescriptorSetConfigure::~DescriptorSetConfigure() = default;

    uint32_t DescriptorSetConfigure::getSet() const {
        return mSet;
    }

//    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> &DescriptorSetConfigure::getVulkanDescriptorBindingConfigures() {
//        return mVulkanDescriptorBindingConfigures;
//    }

    DescriptorSetConfigure &DescriptorSetConfigure::set(uint32_t set) {
        mSet = set;
        return *this;
    }

    // todo
//    DescriptorSetConfigure &DescriptorSetConfigure::addVulkanDescriptor(std::unique_ptr<VulkanDescriptorBindingConfigure> &&vulkanDescriptor) {
//        mVulkanDescriptorBindingConfigures.push_back(std::move(vulkanDescriptor));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mVulkanDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eUniformBuffer, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const std::function<void(VulkanUniformConfigure &)> &configure) {
//        VulkanUniformConfigure config{};
//        configure(config);
//
//        mVulkanDescriptorBindingConfigures[config.mBinding] = std::move(config.createVulkanDescriptorBindingConfigure());
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mVulkanDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eCombinedImageSampler, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(const std::function<void(VulkanSamplerConfigure &)> &configure){
//        VulkanSamplerConfigure config{};
//        configure(config);
//
//        mVulkanDescriptorBindingConfigures[config.mBinding] = std::move(config.createVulkanDescriptorBindingConfigure());
//        return *this;
//    }
//
//    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(const std::function<void(VulkanImmutableSamplerConfigure &)> &configure){
//        VulkanImmutableSamplerConfigure config{};
//        configure(config);
//
//        mVulkanDescriptorBindingConfigures[config.mBinding] = std::move(config.createVulkanDescriptorBindingConfigure());
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mVulkanDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eCombinedImageSampler, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mVulkanDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eStorageBuffer, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }


//    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t size) {
//        VulkanUniformData data{size};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, ImageSize imageSize) {
//        VulkanSamplerData data{imageSize};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    DescriptorSetConfigure &DescriptorSetConfigure::addAndroidHardwareBufferSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, AHardwareBuffer *hardwareBuffer) {
//        VulkanAndroidHardwareBufferSamplerData data{hardwareBuffer};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    const VulkanDescriptorSet &DescriptorSetConfigure::build() const {
//        return mUniformSet;
//    }

//    std::unique_ptr<VulkanDescriptorBindingSet> DescriptorSetConfigure::createVulkanDescriptorBindingSet(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
//        if (mVulkanDescriptorBindingConfigures.empty()) {
//            return nullptr;
//        }
//
//        std::unique_ptr<VulkanDescriptorBindingSet> vulkanDescriptorBindingSet = std::make_unique<VulkanDescriptorBindingSet>();
//
//        for (const auto &entry: mVulkanDescriptorBindingConfigures) {
//            uint32_t binding = entry.first;
//            const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorBindingConfigure = entry.second;
//
//            vulkanDescriptorBindingSet->set(binding, vulkanDescriptorBindingConfigure->createVulkanDescriptorBinding(vulkanDevice, commandPool));
//        }
//
//
//        return vulkanDescriptorBindingSet;
//    }

//    std::vector<vk::DescriptorSetLayoutBinding> DescriptorSetConfigure::createDescriptorSetLayoutBindings() {
//
//        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
//        descriptorSetLayoutBindings.reserve(mVulkanDescriptorBindingConfigures.size());
//
//        for (const auto &entry: mVulkanDescriptorBindingConfigures) {
//            uint32_t binding = entry.first;
//            const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorConfigure = entry.second;
//
//            vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding = vulkanDescriptorConfigure->createDescriptorSetLayoutBinding();
//            descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
//        }
//
////        for (const VulkanUniformConfigure &vulkanUniformConfigure: mVulkanUniformConfigures) {
////            vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding = vulkanUniformConfigure.createDescriptorSetLayoutBinding();
////            descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
////        }
//
//        return descriptorSetLayoutBindings;
//    }

//    vk::DescriptorSetLayout DescriptorSetConfigure::createDescriptorSetLayout(const VulkanDevice &vulkanDevice) {
//
//        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = createDescriptorSetLayoutBindings();
//
//        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
//        descriptorSetLayoutCreateInfo
//                .setBindings(descriptorSetLayoutBindings);
//
//        return vulkanDevice.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
//    }

} // engine