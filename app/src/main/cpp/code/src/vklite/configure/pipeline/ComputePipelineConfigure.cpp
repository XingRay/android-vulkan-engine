//
// Created by leixing on 2025/3/7.
//

#include "ComputePipelineConfigure.h"

namespace vklite {
    ComputePipelineConfigure::ComputePipelineConfigure() {

    }

    ComputePipelineConfigure::~ComputePipelineConfigure() {

    }

    std::unique_ptr<ComputePipeline> ComputePipelineConfigure::build() {
//        return std::make_unique<VulkanComputePipeline>();
        return nullptr;
    }

} // engine