//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/sampler/SamplerInterface.h"

namespace engine {

    class SamplerBuilderInterface {
    public:
        SamplerBuilderInterface();

        virtual ~SamplerBuilderInterface() = 0;

        virtual std::unique_ptr<SamplerInterface> build(const VulkanDevice &vulkanDevice) = 0;
    };

} // engine
