//
// Created by leixing on 2025/1/5.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "ShaderFormat.h"

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

    class VulkanVertexShader {
    private:
        std::vector<char> mCode;

        std::vector<vk::VertexInputBindingDescription> mVertexDescriptions;

        std::vector<vk::VertexInputAttributeDescription> mVertexInputAttributeDescriptions;

        uint32_t mCurrentVertexBinding = -1;

        uint32_t mCurrentVertexLocation = -1;


        std::vector<vk::DescriptorPoolSize> mUniformDescriptorPoolSizes;

        std::vector<vk::DescriptorSetLayoutBinding> mUniformDescriptorSetLayoutBindings;

        uint32_t mCurrentUniformBinding = -1;

        std::vector<uint32_t> mUniformSizes;

    public:
        explicit VulkanVertexShader(std::vector<char> &code);

        ~VulkanVertexShader();

        [[nodiscard]]
        const std::vector<char> &getShaderCode() const;

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

        VulkanVertexShader &addVertexBinding(uint32_t size);

        VulkanVertexShader &addVertexBinding(uint32_t size, uint32_t binding);

        VulkanVertexShader &addVertexAttribute(ShaderFormat format);

        VulkanVertexShader &addVertexAttribute(vk::Format format);

        VulkanVertexShader &addVertexAttribute(uint32_t location, ShaderFormat format, uint32_t offset, uint32_t binding);

        VulkanVertexShader &addVertexAttribute(uint32_t location, vk::Format format, uint32_t offset, uint32_t binding);

        VulkanVertexShader &addUniform(uint32_t size, uint32_t descriptorCount);

        VulkanVertexShader &addUniform(uint32_t size, uint32_t descriptorCount, uint32_t uniformBinding);
    };

} // engine
