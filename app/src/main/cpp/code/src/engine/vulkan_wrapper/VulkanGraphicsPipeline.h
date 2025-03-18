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
#include "engine/vulkan_wrapper/VulkanDescriptorPool.h"

namespace engine {

    class VulkanGraphicsPipeline {
    private:
        const VulkanDevice &mDevice;

        vk::Pipeline mPipeline;
        vk::PipelineLayout mPipelineLayout;

        std::unique_ptr<VulkanDescriptorPool> mVulkanDescriptorPool;
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;

        std::vector<vk::PushConstantRange> mPushConstantRanges;
        std::vector<std::vector<uint8_t>> mPushConstantDataList;

    public:
        VulkanGraphicsPipeline(const VulkanDevice &vulkanDevice,
                               const VulkanSwapchain &swapchain,
                               const VulkanRenderPass &renderPass,
                               const VulkanShaderModule &vertexShaderModule,
                               const VulkanShaderModule &fragmentShaderModule,
                               const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                               const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                               uint32_t frameCount,
                               std::unique_ptr<VulkanDescriptorPool> &&vulkanDescriptorPool,
                               const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                               std::vector<vk::PushConstantRange> &&pushConstantRanges);

        ~VulkanGraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;

        [[nodiscard]]
        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;

        [[nodiscard]]
        const std::vector<std::vector<uint8_t>> &getPushConstantDataList() const;
    };

} // engine
