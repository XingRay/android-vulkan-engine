//
// Created by leixing on 2025/3/7.
//

#include "VulkanGraphicsPipelineConfigure.h"

namespace engine {

    VulkanGraphicsPipelineConfigure::VulkanGraphicsPipelineConfigure() {

    }

    VulkanGraphicsPipelineConfigure::~VulkanGraphicsPipelineConfigure() = default;


    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::vertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::fragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::vertex(const std::function<void(VulkanVertexConfigure &)> &configure) {
        VulkanVertexConfigure builder{};
        configure(builder);
        mVulkanVertexConfigures.push_back(builder);
//        mVertices.push_back(builder.build());
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::uniformSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure) {
        VulkanDescriptorSetConfigure builder{};
        configure(builder);
        mVulkanDescriptorSetConfigures.push_back(builder);
//        mDescriptorSets.push_back(builder.build());
        return *this;
    }

//    // todo: move
//    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::addUniformSet(const VulkanDescriptorSet &uniformSet) {
//        mDescriptorSets.push_back(uniformSet);
//        return *this;
//    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits) {
        mPushConstants.emplace_back(size, offset, stageFlagBits);
        return *this;
    }

    std::unique_ptr<VulkanGraphicsPipeline> VulkanGraphicsPipelineConfigure::build(const VulkanDevice &vulkanDevice,
                                                                                   const VulkanSwapchain &swapchain,
                                                                                   const VulkanRenderPass &renderPass) {
        // shader code

        vk::ShaderModule vertexShaderModule = vulkanDevice.createShaderModule(mVertexShaderCode);
        vk::ShaderModule fragmentShaderModule = vulkanDevice.createShaderModule(mFragmentShaderCode);

        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;

        for (const VulkanVertexConfigure &vertexConfigure: mVulkanVertexConfigures) {
            VulkanVertex vertex = vertexConfigure.build();

            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription
                    .setBinding(vertex.binding)
                    .setStride(vertex.size)
                    .setInputRate(vk::VertexInputRate::eVertex);
            vertexInputBindingDescriptions.push_back(bindingDescription);

            for (const VulkanVertexAttribute &attribute: vertex.attributes) {
                vk::VertexInputAttributeDescription attributeDescription{};
                attributeDescription
                        .setLocation(attribute.location)
                        .setFormat(attribute.format)
                        .setOffset(attribute.offset)
                        .setBinding(attribute.binding);

                vertexInputAttributeDescriptions.push_back(attributeDescription);
            }
        }

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

        for (const VulkanDescriptorSetConfigure &descriptorSetConfigure: mVulkanDescriptorSetConfigures) {
            const VulkanDescriptorSet &descriptorSet = descriptorSetConfigure.build();

            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
            descriptorSetLayoutBindings.reserve(descriptorSet.descriptors.size());

            for (int descriptorIndex = 0; descriptorIndex < descriptorSet.descriptors.size(); descriptorIndex++) {
                const VulkanDescriptor &descriptor = descriptorSet.descriptors[descriptorIndex];

                vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

                descriptorSetLayoutBinding
                        .setBinding(descriptor.getBinding())
                        .setDescriptorType(descriptor.getDescriptorType())
                        .setDescriptorCount(descriptor.getDescriptorCount())
                        .setStageFlags(descriptor.getStageFlags());

                std::vector<VulkanSampler> immutableSamplers = descriptor.getImmutableSamplers();
                if (!immutableSamplers.empty()) {
                    std::vector<vk::Sampler> samplers;
                    for (const VulkanSampler &vulkanSampler: immutableSamplers) {
                        samplers.push_back(vulkanSampler.getSampler());
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


        // push constant
        std::vector<vk::PushConstantRange> pushConstantRanges;
        vk::PhysicalDeviceProperties deviceProperties = vulkanDevice.getPhysicalDevice().getProperties();
        uint32_t maxPushConstantsSize = deviceProperties.limits.maxPushConstantsSize;
        uint32_t totalPushConstantSize = 0;
        for (const VulkanPushConstant &pushConstant: mPushConstants) {
            // 检查单个 Push Constant 的大小是否合理
            if (pushConstant.size == 0) {
                throw std::runtime_error("Push Constant size cannot be zero");
            }

            // 检查偏移和大小是否超出设备限制
            if (pushConstant.offset + pushConstant.size > maxPushConstantsSize) {
                throw std::runtime_error(
                        "Push Constant range (offset: " + std::to_string(pushConstant.offset) +
                        ", size: " + std::to_string(pushConstant.size) +
                        ") exceeds device limit: " + std::to_string(maxPushConstantsSize)
                );
            }

            vk::PushConstantRange pushConstantRange{};
            pushConstantRange
                    .setStageFlags(pushConstant.stageFlagBits)
                    .setOffset(pushConstant.offset)
                    .setSize(pushConstant.size);
            pushConstantRanges.push_back(pushConstantRange);

//            // 创建数据缓冲区
//            std::vector<uint8_t> data(pushConstant.size);
//            mPushConstantDataList.push_back(std::move(data));

            totalPushConstantSize += pushConstant.size;
        }

        // 检查总大小是否超出设备限制
        if (totalPushConstantSize > maxPushConstantsSize) {
            throw std::runtime_error(
                    "Total Push Constant size (" + std::to_string(totalPushConstantSize) +
                    ") exceeds device limit: " + std::to_string(maxPushConstantsSize)
            );
        }

        return std::make_unique<VulkanGraphicsPipeline>(vulkanDevice, swapchain, renderPass,
                                                        vertexShaderModule, fragmentShaderModule,
                                                        vertexInputBindingDescriptions,
                                                        vertexInputAttributeDescriptions,
                                                        descriptorSetLayouts,
                                                        pushConstantRanges);
    }

} // engine