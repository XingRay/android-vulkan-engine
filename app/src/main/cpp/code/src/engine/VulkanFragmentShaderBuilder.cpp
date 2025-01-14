//
// Created by leixing on 2025/1/11.
//

#include "VulkanFragmentShaderBuilder.h"

namespace engine {
    VulkanFragmentShaderBuilder::VulkanFragmentShaderBuilder(VulkanGraphicsEngineBuilder &builder)
            : mBuilder(builder) {

    }

    VulkanFragmentShaderBuilder::~VulkanFragmentShaderBuilder() = default;

    VulkanFragmentShaderBuilder &VulkanFragmentShaderBuilder::code(std::vector<char> &&code) {
        mCode = std::move(code);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanFragmentShaderBuilder::build() {
        mBuilder.setFragmentShaderCode(std::move(mCode));
        return mBuilder;
    }
} // engine