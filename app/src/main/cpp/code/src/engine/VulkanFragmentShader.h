//
// Created by leixing on 2025/1/5.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "ShaderFormat.h"

namespace engine {

    class ImageSize {
    public:
        uint32_t width;
        uint32_t height;
        uint32_t channels;
    };

    class VulkanFragmentShader {
    private:
        std::vector<char> mCode;


        // uniform
        // todo merge with vertex shader
        std::vector<vk::DescriptorPoolSize> mUniformDescriptorPoolSizes;

        std::vector<vk::DescriptorSetLayoutBinding> mUniformDescriptorSetLayoutBindings;

        uint32_t mCurrentUniformBinding = -1;

        std::vector<uint32_t> mUniformSizes;

        std::vector<ImageSize> mSamplerImageSizes;


        // push constant
        vk::PushConstantRange mPushConstantRange{};

    public:
        explicit VulkanFragmentShader(std::vector<char> &code);

        ~VulkanFragmentShader();

        [[nodiscard]]
        const std::vector<char> &getCode() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorPoolSize> &getUniformDescriptorPoolSizes() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSetLayoutBinding> &getUniformDescriptorSetLayoutBindings() const;

        [[nodiscard]]
        const std::vector<uint32_t> &getUniformSizes() const;

        [[nodiscard]]
        const std::vector<ImageSize> &getSamplerImageSizes() const;

        [[nodiscard]]
        const vk::PushConstantRange &getPushConstantRange() const;


        VulkanFragmentShader &addUniform(uint32_t size, uint32_t descriptorCount);

        VulkanFragmentShader &addUniform(uint32_t size, uint32_t descriptorCount, uint32_t uniformBinding);

        VulkanFragmentShader &addSampler(uint32_t width, uint32_t height, uint32_t channels, uint32_t descriptorCount, uint32_t uniformBinding);

        VulkanFragmentShader &setPushConstant(uint32_t size);

    };

} // engine

