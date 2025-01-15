//
// Created by leixing on 2025/1/15.
//

#pragma once

#include "engine/VulkanShaderConfigure.h"

namespace engine {

    class VulkanUniformSetConfigure {
    private:
        VulkanShaderConfigure &mBuilder;

        uint32_t mSet;

    public:

        VulkanUniformSetConfigure(VulkanShaderConfigure& builder);

        ~VulkanUniformSetConfigure();

        VulkanUniformSetConfigure& set(uint32_t set);

        VulkanShaderConfigure& build();
    };

} // engine
