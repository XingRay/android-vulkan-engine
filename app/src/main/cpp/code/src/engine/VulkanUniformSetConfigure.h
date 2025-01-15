//
// Created by leixing on 2025/1/15.
//

#pragma once

#include "engine/VulkanShaderConfigure.h"
#include "engine/VulkanUniformSet.h"

namespace engine {

    class VulkanUniformSetConfigure {
    private:
        VulkanShaderConfigure &mBuilder;

        VulkanUniformSet mUniformSet;

    public:

        VulkanUniformSetConfigure(VulkanShaderConfigure &builder);

        ~VulkanUniformSetConfigure();

        VulkanUniformSetConfigure &set(uint32_t set);

        VulkanUniformSetConfigure &addNormalBinding(uint32_t binding);

        VulkanUniformSetConfigure &addSamplerBinding(uint32_t binding);

        VulkanShaderConfigure &build();
    };

} // engine
