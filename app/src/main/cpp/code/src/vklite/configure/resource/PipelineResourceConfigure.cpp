//
// Created by leixing on 2025/4/25.
//

#include "PipelineResourceConfigure.h"

namespace vklite {

    PipelineResourceConfigure::PipelineResourceConfigure() {

    }

    PipelineResourceConfigure::~PipelineResourceConfigure() {

    }

    PipelineResourceConfigure &PipelineResourceConfigure::index(const std::function<void(IndexBufferConfigure &)> &configure) {
        IndexBufferConfigure config{};
        configure(config);
        mVulkanIndexConfigure = config;
        return *this;
    }

    PipelineResourceConfigure &PipelineResourceConfigure::index(uint32_t capacity) {
        mVulkanIndexConfigure.setIndexBuffer(capacity);
        return *this;
    }

    PipelineResourceConfigure &PipelineResourceConfigure::index(uint32_t capacity, std::vector<uint32_t> &&indices) {
        mVulkanIndexConfigure.setIndexBuffer(capacity, std::move(indices));
        return *this;
    }

    PipelineResourceConfigure &PipelineResourceConfigure::index(std::vector<uint32_t> &&indices) {
        mVulkanIndexConfigure.setIndexBuffer(std::move(indices));
        return *this;
    }

    PipelineResourceConfigure &PipelineResourceConfigure::index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer) {
        mVulkanIndexConfigure.setIndexBuffer(indexBuffer);
        return *this;
    }
    
} // engine