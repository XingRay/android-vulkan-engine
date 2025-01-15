//
// Created by leixing on 2025/1/15.
//

#include "VulkanUniformSetConfigure.h"

namespace engine {

    VulkanUniformSetConfigure::VulkanUniformSetConfigure(VulkanShaderConfigure &builder)
            : mBuilder(builder), mSet(0) {

    }

    VulkanUniformSetConfigure::~VulkanUniformSetConfigure() = default;

    VulkanUniformSetConfigure &VulkanUniformSetConfigure::set(uint32_t set) {
        mSet = set;
        return *this;
    }

    VulkanShaderConfigure &VulkanUniformSetConfigure::build() {
//        mBuilder.addUniformSet();

        return mBuilder;
    }

} // engine