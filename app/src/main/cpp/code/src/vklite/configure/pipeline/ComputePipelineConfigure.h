//
// Created by leixing on 2025/3/7.
//

#pragma once
#include <memory>

#include "vklite/pipeline/ComputePipeline.h"

namespace vklite {

    class ComputePipelineConfigure {

    public:
        ComputePipelineConfigure();
        ~ComputePipelineConfigure();

        std::unique_ptr<ComputePipeline> build();

    };

} // engine
