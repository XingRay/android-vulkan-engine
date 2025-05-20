//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "DescriptorSlotConfigure.h"
#include "vklite/pipeline/descriptor/VulkanDescriptorBindingSets.h"
#include "vklite/pipeline/descriptor/PipelineLayout.h"
#include "vklite/configure/push_constants/PushConstantConfigure.h"

namespace vklite {

    class PipelineLayoutConfigure {
    private:
        // [set/binding] -> Descriptor
        std::unique_ptr<std::unordered_map<uint32_t, std::unordered_map<uint32_t, Descriptor>>> mDescriptors;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

    public:
        PipelineLayoutConfigure();

        ~PipelineLayoutConfigure();

        PipelineLayoutConfigure &addDescriptor(uint32_t set, uint32_t binding, Descriptor &&descriptor);

        PipelineLayoutConfigure &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> createPipelineLayout(const VulkanDevice &vulkanDevice);
    };

} // engine
