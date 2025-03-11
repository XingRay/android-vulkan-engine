//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanSampler {
    public:

        virtual ~VulkanSampler() = default;

        [[nodiscard]]
        virtual vk::Sampler getSampler() const = 0;

//        [[nodiscard]]
//        virtual vk::SamplerYcbcrConversion getConversion() const = 0;
    };

} // engine
