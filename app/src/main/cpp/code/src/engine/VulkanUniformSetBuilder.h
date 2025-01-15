//
// Created by leixing on 2025/1/15.
//

#pragma once

#include "engine/VulkanShaderBuilder.h"

namespace engine {

    class VulkanUniformSetBuilder {
    private:
        VulkanShaderBuilder &mBuilder;

        uint32_t mSet;

    public:

        VulkanUniformSetBuilder(VulkanShaderBuilder& builder);

        ~VulkanUniformSetBuilder();

        VulkanUniformSetBuilder& set(uint32_t set);

        VulkanShaderBuilder& build();
    };

} // engine
