//
// Created by leixing on 2025/1/5.
//

#include "VulkanVertexShader.h"
#include "Log.h"

namespace engine {

    VulkanVertexShader::VulkanVertexShader(std::vector<char> &code) {
        mCode = std::move(code);
    }

    VulkanVertexShader::~VulkanVertexShader() {
        mVertexDescriptions.clear();
        mVertexInputAttributeDescriptions.clear();
        mCode.clear();
    }

    /**
     *
     *              Getters
     *
     */
    const std::vector<char> &VulkanVertexShader::getShaderCode() const {
        return mCode;
    }

    const std::vector<vk::VertexInputBindingDescription> &VulkanVertexShader::getVertexDescriptions() const {
        return mVertexDescriptions;
    }

    const std::vector<vk::VertexInputAttributeDescription> &VulkanVertexShader::getVertexInputAttributeDescriptions() const {
        return mVertexInputAttributeDescriptions;
    }

    const std::vector<vk::DescriptorPoolSize> &VulkanVertexShader::getUniformDescriptorPoolSizes() const {
        return mUniformDescriptorPoolSizes;
    }

    const std::vector<vk::DescriptorSetLayoutBinding> &VulkanVertexShader::getUniformDescriptorSetLayoutBindings() const {
        return mUniformDescriptorSetLayoutBindings;
    }

    const std::vector<uint32_t> &VulkanVertexShader::getUniformSizes() const {
        return mUniformSizes;
    }

    /**
     *
     *              Getters end
     *
     */

    VulkanVertexShader &VulkanVertexShader::addVertexBinding(uint32_t size) {
        return addVertexBinding(size, mCurrentVertexBinding + 1);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexBinding(uint32_t elementSize, uint32_t binding) {
        mCurrentVertexBinding = binding;
        mCurrentVertexLocation = -1; // rest location record

        vk::VertexInputBindingDescription bindingDescription{};
        bindingDescription
                .setBinding(binding)
                .setStride(elementSize)
                .setInputRate(vk::VertexInputRate::eVertex);
        mVertexDescriptions.push_back(bindingDescription);


        return *this;
    }


    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(ShaderFormat format) {
        return addVertexAttribute(mCurrentVertexLocation + 1, format, 0, mCurrentVertexBinding);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(vk::Format format) {
        return addVertexAttribute(mCurrentVertexLocation + 1, format, 0, mCurrentVertexBinding);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(uint32_t location, ShaderFormat format, uint32_t offset, uint32_t binding) {
        vk::Format vkFormat = ShaderFormatHelper::toVkFormat(format);
        return addVertexAttribute(location, vkFormat, offset, binding);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(uint32_t location, vk::Format format, uint32_t offset, uint32_t binding) {
        mCurrentVertexLocation = location;

        LOG_D("attributeDescription, location:%d, format:%d, offset:%d, binding:%d", location, format, offset, binding);
        vk::VertexInputAttributeDescription attributeDescription{};
        attributeDescription
                .setLocation(location)
                .setFormat(format)
                .setOffset(offset)
                .setBinding(binding);

        mVertexInputAttributeDescriptions.push_back(attributeDescription);

        return *this;
    }

    VulkanVertexShader &VulkanVertexShader::addUniform(uint32_t size, uint32_t descriptorCount) {
        return addUniform(size, descriptorCount, mCurrentUniformBinding + 1);
    }

    VulkanVertexShader &VulkanVertexShader::addUniform(uint32_t size, uint32_t descriptorCount, uint32_t uniformBinding) {
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
                .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                .setPImmutableSamplers(nullptr);

        mUniformDescriptorSetLayoutBindings.push_back(uniformBufferObjectLayoutBinding);

        if (mUniformSizes.size() < uniformBinding + 1) {
            mUniformSizes.resize(uniformBinding + 1);
        }
        mUniformSizes[uniformBinding] = size;

        return *this;
    }

} // engine