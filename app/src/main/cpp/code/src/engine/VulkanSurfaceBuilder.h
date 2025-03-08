//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanSurface.h"
#include "engine/vulkan_wrapper/VulkanInstance.h"

namespace engine {
    class VulkanSurfaceBuilder {

    public:
        virtual ~VulkanSurfaceBuilder() = default;

        [[nodiscard]]
        virtual std::unique_ptr<VulkanSurface> build(const VulkanInstance &) const = 0;
    };
}