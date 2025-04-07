//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "VulkanDescriptorSetConfigures.h"

#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    VulkanDescriptorSetConfigures::VulkanDescriptorSetConfigures() = default;

    VulkanDescriptorSetConfigures::~VulkanDescriptorSetConfigures() = default;

    VulkanDescriptorSetConfigures &VulkanDescriptorSetConfigures::addVulkanDescriptorSetConfigure(std::unique_ptr<VulkanDescriptorSetConfigure> &&vulkanDescriptorSetConfigure) {
        mVulkanDescriptorSetConfigures[vulkanDescriptorSetConfigure->getSet()] = std::move(vulkanDescriptorSetConfigure);
        return *this;
    }

    std::vector<vk::DescriptorSetLayout> VulkanDescriptorSetConfigures::createDescriptorSetLayouts(const VulkanDevice &vulkanDevice) const {
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

        for (const auto &entry: mVulkanDescriptorSetConfigures) {
            uint32_t set = entry.first;
            const std::unique_ptr<VulkanDescriptorSetConfigure> &descriptorSetConfigure = entry.second;

            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
            const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> &vulkanDescriptorBindingConfigures = descriptorSetConfigure->getVulkanDescriptorBindingConfigures();
            descriptorSetLayoutBindings.reserve(vulkanDescriptorBindingConfigures.size());

            for (const auto &vulkanDescriptorBindingConfiguresEntry: vulkanDescriptorBindingConfigures) {
                uint32_t binding = vulkanDescriptorBindingConfiguresEntry.first;
                const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorConfigure = vulkanDescriptorBindingConfiguresEntry.second;

                vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

                descriptorSetLayoutBinding
                        .setBinding(vulkanDescriptorConfigure->getBinding())
                        .setDescriptorType(vulkanDescriptorConfigure->getDescriptorType())
                        .setDescriptorCount(vulkanDescriptorConfigure->getDescriptorRange())
                        .setStageFlags(vulkanDescriptorConfigure->getShaderStageFlags());

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

    std::vector<vk::DescriptorPoolSize> VulkanDescriptorSetConfigures::createDescriptorPoolSizes(uint32_t frameCount) const {
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;

        for (const auto &vulkanDescriptorSetConfigureEntry: mVulkanDescriptorSetConfigures) {
            uint32_t set = vulkanDescriptorSetConfigureEntry.first;
            const std::unique_ptr<VulkanDescriptorSetConfigure> &vulkanDescriptorSetConfigure = vulkanDescriptorSetConfigureEntry.second;

            for (const auto &vulkanDescriptorBindingConfigureEntry: vulkanDescriptorSetConfigure->getVulkanDescriptorBindingConfigures()) {
                uint32_t binding = vulkanDescriptorBindingConfigureEntry.first;
                const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorBindingConfigure = vulkanDescriptorBindingConfigureEntry.second;

                const vk::DescriptorType type = vulkanDescriptorBindingConfigure->getDescriptorType();
                const uint32_t count = vulkanDescriptorBindingConfigure->getDescriptorRange() * frameCount;
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

    uint32_t VulkanDescriptorSetConfigures::getSetCount(uint32_t frameCount) const {
        return mVulkanDescriptorSetConfigures.size() * frameCount;
    }

//    vk::DescriptorSetLayoutBinding VulkanDescriptorSetConfigures::createDescriptorSetLayoutBinding() {
//
//    }

    std::unique_ptr<VulkanBufferDescriptorBindingSets> VulkanDescriptorSetConfigures::createVulkanBufferDescriptorBindingSets(const VulkanDevice &vulkanDevice) const {
        if (mVulkanDescriptorSetConfigures.empty()) {
            return nullptr;
        }

        std::unique_ptr<VulkanBufferDescriptorBindingSets> vulkanBufferDescriptorBindingSets = std::make_unique<VulkanBufferDescriptorBindingSets>();

        for (const auto &entry: mVulkanDescriptorSetConfigures) {
            uint32_t set = entry.first;
            const std::unique_ptr<VulkanDescriptorSetConfigure> &vulkanDescriptorSetConfigure = entry.second;

            std::unique_ptr<VulkanBufferDescriptorBindingSet> vulkanBufferDescriptorBindingSet = vulkanDescriptorSetConfigure->createVulkanBufferDescriptorBindingSet(vulkanDevice);
            vulkanBufferDescriptorBindingSets->set(set, std::move(vulkanBufferDescriptorBindingSet));
        }

        return vulkanBufferDescriptorBindingSets;
    }

} // engine