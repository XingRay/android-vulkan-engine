//
// Created by leixing on 2025/4/18.
//

#include "PipelineResource.h"

namespace engine {
    PipelineResource::PipelineResource() {

    }

    PipelineResource::~PipelineResource() {

    }

    const std::vector<vk::Buffer> &PipelineResource::getVertexBuffers() const {
        return mVertexBuffers;
    }

    const std::vector<vk::DeviceSize> &PipelineResource::getVertexBufferOffsets() const {
        return mVertexBufferOffsets;
    }

    const vk::Buffer &PipelineResource::getIndexBuffer() const {
        return mIndexBuffer;
    }

    uint32_t PipelineResource::getIndicesCount() const {
        return mIndicesCount;
    }

    const std::vector<vk::DescriptorSet> &PipelineResource::getDescriptorSets() const {
        return mDescriptorSets;
    }

    const std::vector<PushConstant> &PipelineResource::getPushConstants() const {
        return mPushConstants;
    }

} // engine