//
// Created by leixing on 2024/12/28.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanSwapchain.h"
#include "engine/vulkan_wrapper/VulkanRenderPass.h"
#include "engine/vulkan_wrapper/VulkanShader.h"

namespace engine {

    class VulkanComputePipeline {
    private:
        const VulkanDevice &mDevice;
        vk::Pipeline mPipeline;
        vk::PipelineLayout mPipelineLayout;

    public:
        VulkanComputePipeline(
                const VulkanDevice &vulkanDevice,
                const VulkanShaderModule &computeShaderModule,
                const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                const std::vector<vk::PushConstantRange> &pushConstantRanges);

        ~VulkanComputePipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;
    };

} // engine
