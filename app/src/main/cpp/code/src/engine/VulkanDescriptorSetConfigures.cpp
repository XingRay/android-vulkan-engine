//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "VulkanDescriptorSetConfigures.h"

#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {
    VulkanDescriptorSetConfigures::VulkanDescriptorSetConfigures() {

    }

    VulkanDescriptorSetConfigures::~VulkanDescriptorSetConfigures() {

    }

    VulkanDescriptorSetConfigures &VulkanDescriptorSetConfigures::addVulkanDescriptorSetConfigure(std::unique_ptr<VulkanDescriptorSetConfigure> &&vulkanDescriptorSetConfigure) {
        mVulkanDescriptorSetConfigures.push_back(std::move(vulkanDescriptorSetConfigure));
        return *this;
    }

    std::vector<vk::DescriptorSetLayout> VulkanDescriptorSetConfigures::createDescriptorSetLayouts(const VulkanDevice &vulkanDevice) {
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

        for (const std::unique_ptr<VulkanDescriptorSetConfigure> &descriptorSetConfigure: mVulkanDescriptorSetConfigures) {
            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
            descriptorSetLayoutBindings.reserve(descriptorSetConfigure->getVulkanDescriptorConfigures().size());

            for (const std::unique_ptr<VulkanDescriptorConfigure> &vulkanDescriptorConfigure: descriptorSetConfigure->getVulkanDescriptorConfigures()) {
                vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

                descriptorSetLayoutBinding
                        .setBinding(vulkanDescriptorConfigure->getBinding())
                        .setDescriptorType(vulkanDescriptorConfigure->getDescriptorType())
                        .setDescriptorCount(vulkanDescriptorConfigure->getDescriptorCount())
                        .setStageFlags(vulkanDescriptorConfigure->getStageFlags());

                const std::vector<std::unique_ptr<VulkanSampler>> &immutableSamplers = vulkanDescriptorConfigure->getImmutableSamplers();
                if (!immutableSamplers.empty()) {
                    std::vector<vk::Sampler> samplers;
                    for (const std::unique_ptr<VulkanSampler> &vulkanSampler: immutableSamplers) {
                        samplers.push_back(vulkanSampler->getSampler());
                    }

                    descriptorSetLayoutBinding
                            .setDescriptorCount(samplers.size())
                            .setPImmutableSamplers(samplers.data());
                }

                descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
            }

            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
            descriptorSetLayoutCreateInfo
                    .setBindings(descriptorSetLayoutBindings);

            vk::DescriptorSetLayout descriptorSetLayout = vulkanDevice.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
            descriptorSetLayouts.push_back(descriptorSetLayout);
        }

        return descriptorSetLayouts;
    }

    std::vector<vk::DescriptorPoolSize> VulkanDescriptorSetConfigures::createDescriptorPoolSizes() {
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;
        for (const std::unique_ptr<VulkanDescriptorSetConfigure> &vulkanDescriptorSetConfigure: mVulkanDescriptorSetConfigures) {
            for (const std::unique_ptr<VulkanDescriptorConfigure> &vulkanDescriptorConfigures: vulkanDescriptorSetConfigure->getVulkanDescriptorConfigures()) {
                const vk::DescriptorType type = vulkanDescriptorConfigures->getDescriptorType();
                const uint32_t count = vulkanDescriptorConfigures->getDescriptorCount();
                if (descriptorTypeToIndexMap.contains(type)) {
                    descriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
                } else {
                    descriptorTypeToIndexMap[type] = descriptorPoolSizes.size();
                    descriptorPoolSizes.emplace_back(type, count);
                }
            }
        }

        return descriptorPoolSizes;
    }

} // engine