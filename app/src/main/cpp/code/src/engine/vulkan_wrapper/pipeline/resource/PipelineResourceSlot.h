//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/pipeline/resource/BufferInfo.h"
#include "engine/vulkan_wrapper/pipeline/resource/ImageInfo.h"

namespace engine {

    class PipelineResourceSlot {
    private:
        // buffer info
        std::unique_ptr<BufferInfo> mVulkanDescriptorBufferInfo;

        // image info
        std::unique_ptr<ImageInfo> mVulkanDescriptorImageInfo;

        // immutable sampler
        std::unique_ptr<ImageInfo> mImmutableSamplerInfo;

    public:
        PipelineResourceSlot();

        PipelineResourceSlot(PipelineResourceSlot &&other) noexcept;

        ~PipelineResourceSlot();
    };

} // engine
