//
// Created by leixing on 2024/12/28.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanSwapchain.h"
#include "engine/vulkan_wrapper/VulkanRenderPass.h"
#include "engine/vulkan_wrapper/VulkanShader.h"
#include "engine/vulkan_wrapper/VulkanShaderModule.h"

namespace engine {

    class VulkanGraphicsPipeline {
    private:
        const VulkanDevice &mDevice;
        vk::Pipeline mPipeline;
        vk::PipelineLayout mPipelineLayout;

    public:
        VulkanGraphicsPipeline(const VulkanDevice &vulkanDevice,
                               const VulkanSwapchain &swapchain,
                               const VulkanRenderPass &renderPass,
                               const VulkanShaderModule &vertexShaderModule,
                               const VulkanShaderModule &fragmentShaderModule,
                               const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                               const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                               const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                               const std::vector<vk::PushConstantRange> &pushConstantRanges);

        ~VulkanGraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;
    };

} // engine
