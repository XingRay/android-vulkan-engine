//
// Created by leixing on 2024/12/28.
//

#include "engine/vulkan_wrapper/VulkanComputePipeline.h"
#include "engine/Log.h"

namespace engine {
    VulkanComputePipeline::VulkanComputePipeline(const VulkanDevice &vulkanDevice,
                                                 const VulkanShaderModule &computeShaderModule,
                                                 const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                                                 const std::vector<vk::PushConstantRange> &pushConstantRanges)
            : mDevice(vulkanDevice) {
        vk::Device device = vulkanDevice.getDevice();

        vk::PipelineShaderStageCreateInfo computeShaderStageInfo{};
        computeShaderStageInfo
                .setStage(vk::ShaderStageFlagBits::eCompute)
                .setModule(computeShaderModule.getShaderModule())
                .setPName("main");

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo
                .setSetLayouts(descriptorSetLayouts)
                .setPushConstantRanges(pushConstantRanges);

        mPipelineLayout = vulkanDevice.getDevice().createPipelineLayout(pipelineLayoutCreateInfo);

        vk::ComputePipelineCreateInfo computePipelineCreateInfo{};
        computePipelineCreateInfo
                .setLayout(mPipelineLayout)
                .setStage(computeShaderStageInfo);

        auto [result, pipeline] = device.createComputePipeline(nullptr, computePipelineCreateInfo);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("createComputePipeline failed");
        }
        mPipeline = pipeline;
    }

    VulkanComputePipeline::~VulkanComputePipeline() {
        LOG_D("VulkanGraphicsPipeline::~VulkanGraphicsPipeline");
        vk::Device device = mDevice.getDevice();
        device.destroy(mPipeline);
        device.destroy(mPipelineLayout);
    }

    const vk::Pipeline &VulkanComputePipeline::getPipeline() const {
        return mPipeline;
    }

    const vk::PipelineLayout &VulkanComputePipeline::getPipelineLayout() const {
        return mPipelineLayout;
    }
} // engine