//
// Created by leixing on 2025/3/30.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace vklite {

    class SamplerInterface {
    private:

    public:
        SamplerInterface();

        virtual ~SamplerInterface() = 0;

        [[nodiscard]]
        virtual const vk::Sampler &getSampler() const = 0;
    };

} // engine
