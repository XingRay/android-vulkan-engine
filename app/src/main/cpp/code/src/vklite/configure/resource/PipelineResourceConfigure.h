//
// Created by leixing on 2025/4/25.
//

#pragma once

#include "vklite/vertex/buffer/VertexBufferConfigure.h"
#include "vklite/configure/index_buffer/IndexBufferConfigure.h"

namespace vklite {

    class PipelineResourceConfigure {
    private:

        /**
         * vertex buffer
         */
        VertexBufferConfigure mVertexBufferConfigure;
        /**
         * index buffer
         */
        IndexBufferConfigure mVulkanIndexConfigure;

    public:
        PipelineResourceConfigure();

        ~PipelineResourceConfigure();

        /**
         * index
         */
        PipelineResourceConfigure &index(const std::function<void(IndexBufferConfigure &)> &configure);

        PipelineResourceConfigure &index(uint32_t capacity);

        PipelineResourceConfigure &index(uint32_t capacity, std::vector<uint32_t> &&indices);

        PipelineResourceConfigure &index(std::vector<uint32_t> &&indices);

        PipelineResourceConfigure &index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer);
    };

} // engine
