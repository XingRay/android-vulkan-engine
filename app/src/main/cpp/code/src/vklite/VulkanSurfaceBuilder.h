//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <memory>

#include "VulkanSurface.h"
#include "vklite/instance/VulkanInstance.h"

namespace vklite {
    class VulkanSurfaceBuilder {

    public:
        virtual ~VulkanSurfaceBuilder() = default;

        [[nodiscard]]
        virtual std::unique_ptr<VulkanSurface> build(const VulkanInstance &) const = 0;
    };
}