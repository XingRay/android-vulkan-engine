//
// Created by leixing on 2025/1/5.
//

#include "VulkanVertexShader.h"

namespace engine {

    VulkanVertexShader::VulkanVertexShader(const std::vector<char> &code) {
        mCode = code;
    }

    VulkanVertexShader::~VulkanVertexShader() {
        mVertexDescriptions.clear();
        mVertexInputAttributeDescriptions.clear();
        mCode.clear();
    }

    const std::vector<char> &VulkanVertexShader::getShaderCode() const {
        return mCode;
    }

    const std::vector<vk::VertexInputBindingDescription> &VulkanVertexShader::getVertexDescriptions() const {
        return mVertexDescriptions;
    }

    const std::vector<vk::VertexInputAttributeDescription> &VulkanVertexShader::getVertexInputAttributeDescriptions() const {
        return mVertexInputAttributeDescriptions;
    }

    VulkanVertexShader &VulkanVertexShader::addVertexBinding(uint32_t size) {
        return addVertexBinding(size, mCurrentBinding + 1);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexBinding(uint32_t elementSize, uint32_t binding) {
        mCurrentBinding = binding;
        mCurrentLocation = -1; // rest location record

        vk::VertexInputBindingDescription bindingDescription{};
        bindingDescription
                .setBinding(binding)
                .setStride(elementSize)
                .setInputRate(vk::VertexInputRate::eVertex);
        mVertexDescriptions.push_back(bindingDescription);


        return *this;
    }


    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(ShaderFormat format) {
        return addVertexAttribute(mCurrentLocation + 1, format, 0, mCurrentBinding);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(vk::Format format) {
        return addVertexAttribute(mCurrentLocation + 1, format, 0, mCurrentBinding);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(uint32_t location, ShaderFormat format, uint32_t offset, uint32_t binding) {
        vk::Format vkFormat = ShaderFormatHelper::toVkFormat(format);
        return addVertexAttribute(location, vkFormat, offset, binding);
    }

    VulkanVertexShader &VulkanVertexShader::addVertexAttribute(uint32_t location, vk::Format format, uint32_t offset, uint32_t binding) {
        mCurrentLocation = location;

        vk::VertexInputAttributeDescription attributeDescription{};
        attributeDescription.binding = binding;
        attributeDescription.location = location;
        attributeDescription.format = format;
        attributeDescription.offset = offset;

        mVertexInputAttributeDescriptions.push_back(attributeDescription);

        return *this;
    }

} // engine