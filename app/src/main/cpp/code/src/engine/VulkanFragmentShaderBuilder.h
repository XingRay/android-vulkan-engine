//
// Created by leixing on 2025/1/11.
//

#pragma once

#include "engine/VulkanGraphicsEngineBuilder.h"

namespace engine {

    class VulkanGraphicsEngineBuilder;

    class VulkanFragmentShaderBuilder {
    private:
        VulkanGraphicsEngineBuilder &mBuilder;

        std::vector<char> mCode;

    public:

        VulkanFragmentShaderBuilder(VulkanGraphicsEngineBuilder &builder);

        ~VulkanFragmentShaderBuilder();

        VulkanFragmentShaderBuilder & code(std::vector<char> && code);

        VulkanGraphicsEngineBuilder &build();
    };

} // engine
