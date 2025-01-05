//
// Created by leixing on 2025/1/5.
//

#include "VulkanFragmentShader.h"

namespace engine {

    VulkanFragmentShader::VulkanFragmentShader(std::vector<char> code) {
        mCode = code;
    }

    VulkanFragmentShader::~VulkanFragmentShader() {

    }

    const std::vector<char> &VulkanFragmentShader::getShaderCode() const {
        return mCode;
    }

} // engine