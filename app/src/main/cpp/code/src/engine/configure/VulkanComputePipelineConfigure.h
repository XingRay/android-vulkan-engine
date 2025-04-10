//
// Created by leixing on 2025/3/7.
//

#pragma once
#include <memory>

#include "engine/vulkan_wrapper/VulkanComputePipeline.h"

namespace engine {

    class VulkanComputePipelineConfigure {

    public:
        VulkanComputePipelineConfigure();
        ~VulkanComputePipelineConfigure();

        std::unique_ptr<VulkanComputePipeline> build();

    };

} // engine
