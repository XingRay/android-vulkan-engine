//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "PipelineResourceSlot.h"
#include "vklite/push_constant/PushConstant.h"
/**
 * resources for a frame used in pipeline
 * resources: uniform buffer, vk::ImageView ...
 */
namespace vklite {

    class PipelineResource {
    private:

        std::vector<vk::Buffer> mVertexBuffers;

        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        vk::Buffer mIndexBuffer;

        uint32_t mIndicesCount;

        // set -> binding -> offset -> slot
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::unordered_map<uint32_t, PipelineResourceSlot>>> mPipelineResourceSlots;

        std::vector<vk::DescriptorSet> mDescriptorSets;

        std::vector<PushConstant> mPushConstants;

    public:

        PipelineResource();

        ~PipelineResource();

        [[nodiscard]]
        const std::vector<vk::Buffer> &getVertexBuffers() const;

        [[nodiscard]]
        const std::vector<vk::DeviceSize> &getVertexBufferOffsets() const;

        [[nodiscard]]
        const vk::Buffer &getIndexBuffer() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSet> &getDescriptorSets() const;

        [[nodiscard]]
        const std::vector<PushConstant> &getPushConstants() const;

    };

} // engine
