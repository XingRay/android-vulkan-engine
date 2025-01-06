//
// Created by leixing on 2025/1/5.
//

#include "VulkanFragmentShader.h"

namespace engine {

    VulkanFragmentShader::VulkanFragmentShader(std::vector<char> &code) {
        mCode = std::move(code);
    }

    VulkanFragmentShader::~VulkanFragmentShader() {
        mCode.clear();
    }

    const std::vector<char> &VulkanFragmentShader::getCode() const {
        return mCode;
    }

    const vk::PushConstantRange &VulkanFragmentShader::getPushConstantRange() const {
        return mPushConstantRange;
    }


    VulkanFragmentShader &VulkanFragmentShader::setPushConstant(uint32_t size) {
        mPushConstantRange
                .setStageFlags(vk::ShaderStageFlagBits::eFragment)
                .setSize(size)
                .setOffset(0);

        return *this;
    }

} // engine