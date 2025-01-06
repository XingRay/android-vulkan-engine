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

    const std::vector<vk::DescriptorPoolSize> &VulkanFragmentShader::getUniformDescriptorPoolSizes() const {
        return mUniformDescriptorPoolSizes;
    }

    const std::vector<vk::DescriptorSetLayoutBinding> &VulkanFragmentShader::getUniformDescriptorSetLayoutBindings() const {
        return mUniformDescriptorSetLayoutBindings;
    }

    const std::vector<uint32_t> &VulkanFragmentShader::getUniformSizes() const {
        return mUniformSizes;
    }

    const std::vector<ImageSize> &VulkanFragmentShader::getSamplerImageSizes() const {
        return mSamplerImageSizes;
    }

    const vk::PushConstantRange &VulkanFragmentShader::getPushConstantRange() const {
        return mPushConstantRange;
    }


    VulkanFragmentShader &VulkanFragmentShader::addUniform(uint32_t size, uint32_t descriptorCount) {
        return addUniform(size, descriptorCount, mCurrentUniformBinding + 1);
    }

    VulkanFragmentShader &VulkanFragmentShader::addUniform(uint32_t size, uint32_t descriptorCount, uint32_t uniformBinding) {
        mCurrentUniformBinding = uniformBinding;

        vk::DescriptorPoolSize uniformBufferObjectPoolSize{};
        uniformBufferObjectPoolSize
                .setType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(descriptorCount);

        mUniformDescriptorPoolSizes.push_back(uniformBufferObjectPoolSize);

        vk::DescriptorSetLayoutBinding uniformBufferObjectLayoutBinding{};
        uniformBufferObjectLayoutBinding
                .setBinding(uniformBinding)
                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(1)
                .setStageFlags(vk::ShaderStageFlagBits::eFragment)
                .setPImmutableSamplers(nullptr);

        mUniformDescriptorSetLayoutBindings.push_back(uniformBufferObjectLayoutBinding);

        if (mUniformSizes.size() < uniformBinding + 1) {
            mUniformSizes.resize(uniformBinding + 1);
        }
        mUniformSizes[uniformBinding] = size;

        return *this;
    }


    VulkanFragmentShader &VulkanFragmentShader::addSampler(uint32_t width, uint32_t height, uint32_t channels, uint32_t descriptorCount, uint32_t uniformBinding) {
        mCurrentUniformBinding = uniformBinding;

        vk::DescriptorPoolSize samplerPoolSize{};
        samplerPoolSize
                .setType(vk::DescriptorType::eCombinedImageSampler)
                .setDescriptorCount(descriptorCount);

        mUniformDescriptorPoolSizes.push_back(samplerPoolSize);

        vk::DescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding
                .setBinding(uniformBinding)
                .setDescriptorCount(1)
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setPImmutableSamplers(nullptr)
                .setStageFlags(vk::ShaderStageFlagBits::eFragment);

        mUniformDescriptorSetLayoutBindings.push_back(samplerLayoutBinding);

        if (mSamplerImageSizes.size() < uniformBinding + 1) {
            mSamplerImageSizes.resize(uniformBinding + 1);
        }
        mSamplerImageSizes[uniformBinding] = {width, height, channels};

        return *this;
    }

    VulkanFragmentShader &VulkanFragmentShader::setPushConstant(uint32_t size) {
        mPushConstantRange
                .setStageFlags(vk::ShaderStageFlagBits::eFragment)
                .setSize(size)
                .setOffset(0);

        return *this;
    }

} // engine