//
// Created by leixing on 2025/4/18.
//

#include "PipelineLayout.h"

namespace vklite {

    PipelineLayout::PipelineLayout(const VulkanDevice &vulkanDevice,
                                   std::unique_ptr<std::unordered_map<uint32_t, std::unordered_map<uint32_t, Descriptor>>> &&descriptors,
                                   std::vector<vk::PushConstantRange> &&pushConstantRanges)
            : mVulkanDevice(vulkanDevice), mDescriptors(std::move(descriptors)), mPushConstantRanges(std::move(pushConstantRanges)) {

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = createDescriptorSetLayouts();

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo
                .setSetLayouts(descriptorSetLayouts)
                .setPushConstantRanges(mPushConstantRanges);

        mPipelineLayout = vulkanDevice.getDevice().createPipelineLayout(pipelineLayoutCreateInfo);
    }

    PipelineLayout::~PipelineLayout() {
        vk::Device device = mVulkanDevice.getDevice();

        device.destroy(mPipelineLayout);
    }

    const vk::PipelineLayout &PipelineLayout::getPipelineLayout() const {
        return mPipelineLayout;
    }

    std::vector<vk::DescriptorSetLayout> PipelineLayout::createDescriptorSetLayouts() const {
        const vk::Device &device = mVulkanDevice.getDevice();
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

        for (const auto &setEntry: *mDescriptors) {
            uint32_t set = setEntry.first;
            const std::unordered_map<uint32_t, Descriptor> &descriptorSet = setEntry.second;

            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
            descriptorSetLayoutBindings.reserve(descriptorSet.size());

            for (const auto &bindingEntry: descriptorSet) {
                uint32_t binding = bindingEntry.first;
                const Descriptor &descriptor = bindingEntry.second;

                vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

                descriptorSetLayoutBinding
                        .setBinding(binding)
                        .setDescriptorType(descriptor.getDescriptorType())
                        .setDescriptorCount(descriptor.getDescriptorCount())
                        .setStageFlags(descriptor.getShaderStageFlags());

                const std::vector<vk::Sampler> &immutableSamplers = descriptor.getImmutableVkSamplers();
                if (!immutableSamplers.empty()) {
                    descriptorSetLayoutBinding.setImmutableSamplers(immutableSamplers);
                }

                descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
            }

            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
            descriptorSetLayoutCreateInfo
                    .setBindings(descriptorSetLayoutBindings);

            vk::DescriptorSetLayout descriptorSetLayout = device.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
            descriptorSetLayouts.push_back(descriptorSetLayout);
        }

        return descriptorSetLayouts;
    }

    std::unique_ptr<DescriptorPool> PipelineLayout::createDescriptorPool(uint32_t frameCount) const {
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;

        for (const auto &setEntry: *mDescriptors) {
            uint32_t set = setEntry.first;
            const std::unordered_map<uint32_t, Descriptor> &descriptorSet = setEntry.second;

            for (const auto &bindingEntry: descriptorSet) {
                uint32_t binding = bindingEntry.first;
                const Descriptor &descriptor = bindingEntry.second;

                const vk::DescriptorType type = descriptor.getDescriptorType();
                const uint32_t count = descriptor.getDescriptorCount() * frameCount;
                if (descriptorTypeToIndexMap.contains(type)) {
                    descriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
                } else {
                    descriptorTypeToIndexMap[type] = descriptorPoolSizes.size();
                    descriptorPoolSizes.emplace_back(type, count);
                }
            }
        }

        return std::make_unique<DescriptorPool>(mVulkanDevice, descriptorPoolSizes, (*mDescriptors).size() * frameCount);
    }

    uint32_t PipelineLayout::calcTotalPushConstantSize() const {
        uint32_t totalPushConstantSize = 0;
        for (const vk::PushConstantRange &pushConstantRange: mPushConstantRanges) {
            totalPushConstantSize += pushConstantRange.size;
        }
        return totalPushConstantSize;
    }

} // engine