//
// Created by leixing on 2025/3/30.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanSamplerInterface {
    private:

    public:
        VulkanSamplerInterface();

        virtual ~VulkanSamplerInterface() = 0;

        [[nodiscard]]
        virtual const vk::Sampler &getSampler() const = 0;
    };

} // engine
