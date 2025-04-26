//
// Created by leixing on 2025/3/7.
//

#pragma once
#include <memory>

#include "engine/vulkan_wrapper/pipeline/ComputePipeline.h"

namespace engine {

    class ComputePipelineConfigure {

    public:
        ComputePipelineConfigure();
        ~ComputePipelineConfigure();

        std::unique_ptr<ComputePipeline> build();

    };

} // engine
