//
// Created by leixing on 2025/1/5.
//

#include "VulkanShader.h"
#include "engine/Log.h"

namespace engine {

    VulkanShader::VulkanShader() = default;

    VulkanShader::~VulkanShader() = default;

    const std::vector<char> &VulkanShader::getVertexShaderCode() const {
        return mVertexShaderCode;
    }

    void VulkanShader::setVertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
    }

    const std::vector<char> &VulkanShader::getFragmentShaderCode() const {
        return mFragmentShaderCode;
    }

    void VulkanShader::setFragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
    }

    const std::vector<vk::VertexInputBindingDescription> &VulkanShader::getVertexDescriptions() const {
        return mVertexDescriptions;
    }

    const vk::PushConstantRange &VulkanShader::getVertexPushConstantRange() const {
        return mVertexPushConstantRange;
    }

    const vk::PushConstantRange &VulkanShader::getFragmentPushConstantRange() const {
        return mFragmentPushConstantRange;
    }

    const std::vector<vk::VertexInputAttributeDescription> &VulkanShader::getVertexInputAttributeDescriptions() const {
        return mVertexInputAttributeDescriptions;
    }

    const std::vector<vk::DescriptorPoolSize> &VulkanShader::getUniformDescriptorPoolSizes() const {
        return mUniformDescriptorPoolSizes;
    }

    const std::vector<vk::DescriptorSetLayoutBinding> &VulkanShader::getUniformDescriptorSetLayoutBindings() const {
        return mUniformDescriptorSetLayoutBindings;
    }

    const std::vector<std::unique_ptr<VulkanUniform>> &VulkanShader::getUniforms() const {
        return mUniforms;
    }

    /**
     *
     *              Getters end
     *
     */


    VulkanShader &VulkanShader::setVertexPushConstant(uint32_t size, uint32_t offset) {
        mVertexPushConstantRange
                .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                .setSize(size)
                .setOffset(offset);
        return *this;
    }

    VulkanShader &VulkanShader::setFragmentPushConstant(uint32_t size, uint32_t offset) {
        mVertexPushConstantRange
                .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                .setSize(size)
                .setOffset(offset);
        return *this;
    }

    VulkanShader &VulkanShader::addVertexBinding(uint32_t binding, uint32_t elementSize) {
        vk::VertexInputBindingDescription bindingDescription{};
        bindingDescription
                .setBinding(binding)
                .setStride(elementSize)
                .setInputRate(vk::VertexInputRate::eVertex);
        mVertexDescriptions.push_back(bindingDescription);

        return *this;
    }

    VulkanShader &VulkanShader::addVertexAttribute(uint32_t binding, uint32_t location, vk::Format format, uint32_t offset) {
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


    VulkanShader &VulkanShader::addUniform(uint32_t size, uint32_t descriptorCount, uint32_t uniformBinding, vk::ShaderStageFlags stageFlags) {
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
                .setStageFlags(stageFlags) // vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment
                .setPImmutableSamplers(nullptr);

        mUniformDescriptorSetLayoutBindings.push_back(uniformBufferObjectLayoutBinding);

//        if (mUniformSizes.size() < uniformBinding + 1) {
//            mUniformSizes.resize(uniformBinding + 1);
//        }
//        mUniformSizes[uniformBinding] = size;

        return *this;
    }

} // engine