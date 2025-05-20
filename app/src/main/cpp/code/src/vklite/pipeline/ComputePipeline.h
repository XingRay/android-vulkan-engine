//
// Created by leixing on 2024/12/28.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/shader/VulkanShaderModule.h"

namespace vklite {

    class ComputePipeline {
    private:
        const VulkanDevice &mDevice;
        vk::Pipeline mPipeline;
        vk::PipelineLayout mPipelineLayout;

    public:
        ComputePipeline(
                const VulkanDevice &vulkanDevice,
                const VulkanShaderModule &computeShaderModule,
                const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                const std::vector<vk::PushConstantRange> &pushConstantRanges);

        ~ComputePipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;
    };

} // engine
