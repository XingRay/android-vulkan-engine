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

        uint32_t mCurrentBinding = -1;

        uint32_t mCurrentLocation = -1;

    public:
        explicit VulkanVertexShader(const std::vector<char> &code);

        ~VulkanVertexShader();

        [[nodiscard]]
        const std::vector<char> &getShaderCode() const;

        [[nodiscard]]
        const std::vector<vk::VertexInputBindingDescription> &getVertexDescriptions() const;

        [[nodiscard]]
        const std::vector<vk::VertexInputAttributeDescription> &getVertexInputAttributeDescriptions() const;

        VulkanVertexShader &addVertexBinding(uint32_t size);

        VulkanVertexShader &addVertexBinding(uint32_t size, uint32_t binding);

        VulkanVertexShader &addVertexAttribute(ShaderFormat format);

        VulkanVertexShader &addVertexAttribute(vk::Format format);

        VulkanVertexShader &addVertexAttribute(uint32_t location, ShaderFormat format, uint32_t offset, uint32_t binding);

        VulkanVertexShader &addVertexAttribute(uint32_t location, vk::Format format, uint32_t offset, uint32_t binding);
    };

} // engine
