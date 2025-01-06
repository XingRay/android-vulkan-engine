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

        // push constant
        vk::PushConstantRange mPushConstantRange{};

    public:
        explicit VulkanFragmentShader(std::vector<char> &code);

        ~VulkanFragmentShader();

        [[nodiscard]]
        const std::vector<char> &getCode() const;

        [[nodiscard]]
        const vk::PushConstantRange &getPushConstantRange() const;


        VulkanFragmentShader &setPushConstant(uint32_t size);

    };

} // engine

