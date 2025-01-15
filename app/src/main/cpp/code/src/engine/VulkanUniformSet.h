//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>

#include "engine/VulkanUniform.h"

namespace engine {

    class VulkanUniformSet {
    public:
        uint32_t set;
        std::vector<VulkanUniform> uniforms;
    };

} // engine
