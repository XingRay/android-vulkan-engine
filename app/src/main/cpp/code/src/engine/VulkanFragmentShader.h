//
// Created by leixing on 2025/1/5.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "ShaderFormat.h"

namespace engine {

    class VulkanFragmentShader {
    private:
        std::vector<char> mCode;

    public:
        VulkanFragmentShader(std::vector<char> code);

        ~VulkanFragmentShader();

        [[nodiscard]]
        const std::vector<char> &getShaderCode() const;
    };

} // engine

