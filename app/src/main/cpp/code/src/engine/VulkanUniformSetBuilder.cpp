//
// Created by leixing on 2025/1/15.
//

#include "VulkanUniformSetBuilder.h"

namespace engine {

    VulkanUniformSetBuilder::VulkanUniformSetBuilder(VulkanShaderBuilder &builder)
            : mBuilder(builder), mSet(0) {

    }

    VulkanUniformSetBuilder::~VulkanUniformSetBuilder() = default;

    VulkanUniformSetBuilder &VulkanUniformSetBuilder::set(uint32_t set) {
        mSet = set;
        return *this;
    }

    VulkanShaderBuilder &VulkanUniformSetBuilder::build() {
//        mBuilder.addUniformSet();

        return mBuilder;
    }

} // engine