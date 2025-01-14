//
// Created by leixing on 2025/1/5.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/ShaderFormat.h"

/**
 *

 data:

 struct Vertex {
    glm::vec3 pos;
 };

 code:

 vertexShader->addVertexBinding(sizeof(app::Vertex))
    //.addVertexAttribute(vk::Format::eR32G32B32Sfloat);
    .addVertexAttribute(ShaderFormat::Vec3);

 */
namespace engine {

    class ImageSize {
    public:
        uint32_t width;
        uint32_t height;
        uint32_t channels;
    };

    class VulkanShader {
    private:
        std::vector<char> mVertexShaderCode;
        std::vector<char> mFragmentShaderCode;

        // vertex
        std::vector<vk::VertexInputBindingDescription> mVertexDescriptions;

        std::vector<vk::VertexInputAttributeDescription> mVertexInputAttributeDescriptions;

        // uniform
        std::vector<vk::DescriptorPoolSize> mUniformDescriptorPoolSizes;

        std::vector<vk::DescriptorSetLayoutBinding> mUniformDescriptorSetLayoutBindings;

        std::vector<uint32_t> mUniformSizes;

        std::vector<ImageSize> mSamplerImageSizes;

        // push constant
        vk::PushConstantRange mVertexPushConstantRange{vk::ShaderStageFlagBits::eVertex, 0, 0};

        vk::PushConstantRange mFragmentPushConstantRange{vk::ShaderStageFlagBits::eFragment, 0, 0};

    public:
        explicit VulkanShader();

        ~VulkanShader();

        [[nodiscard]]
        const std::vector<char> &getVertexShaderCode() const;

        void setVertexShaderCode(std::vector<char> &&code);


        [[nodiscard]]
        const std::vector<char> &getFragmentShaderCode() const;

        void setFragmentShaderCode(std::vector<char> &&code);


        [[nodiscard]]
        const vk::PushConstantRange &getVertexPushConstantRange() const;

        [[nodiscard]]
        const vk::PushConstantRange &getFragmentPushConstantRange() const;

        [[nodiscard]]
        const std::vector<vk::VertexInputBindingDescription> &getVertexDescriptions() const;

        [[nodiscard]]
        const std::vector<vk::VertexInputAttributeDescription> &getVertexInputAttributeDescriptions() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorPoolSize> &getUniformDescriptorPoolSizes() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSetLayoutBinding> &getUniformDescriptorSetLayoutBindings() const;

        [[nodiscard]]
        const std::vector<uint32_t> &getUniformSizes() const;

        [[nodiscard]]
        const std::vector<ImageSize> &getSamplerImageSizes() const;


        VulkanShader &setVertexPushConstant(uint32_t size, uint32_t offset);

        VulkanShader &setFragmentPushConstant(uint32_t size, uint32_t offset);

        VulkanShader &addVertexBinding(uint32_t binding, uint32_t elementSize);

        VulkanShader &addVertexAttribute(uint32_t binding, uint32_t location, vk::Format format, uint32_t offset);

        VulkanShader &addUniform(uint32_t size, uint32_t descriptorCount, uint32_t uniformBinding, vk::ShaderStageFlags stageFlags);


    };

} // engine
