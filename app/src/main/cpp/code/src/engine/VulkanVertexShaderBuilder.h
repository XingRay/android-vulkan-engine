//
// Created by leixing on 2025/1/11.
//

#pragma once

#include <functional>

#include "engine/ShaderFormat.h"
#include "engine/VulkanGraphicsEngineBuilder.h"

namespace engine {

    class VulkanGraphicsEngineBuilder;

    class VulkanVertexShaderBuilder;

    class VertexAttribute {
    public:
        uint32_t binding;
        uint32_t location;
        vk::Format format;
        uint32_t offset;
    };

    class Vertex {
    public:
        uint32_t binding;
        uint32_t size;
        std::vector<VertexAttribute> attributes;
    };

    class VertexBuilder {
    private:
        VulkanVertexShaderBuilder &mBuilder;

        uint32_t mSize;
        uint32_t mBinding;
        std::vector<VertexAttribute> mAttributes;

        uint32_t mCurrentLocation = -1;
        uint32_t mCurrentOffset = 0;

    public:
        VertexBuilder(VulkanVertexShaderBuilder &builder);

        VertexBuilder &size(uint32_t size);

        VertexBuilder &binding(uint32_t binding);

        VertexBuilder &addAttribute(ShaderFormat format);

        VertexBuilder &addAttribute(vk::Format format);

        VertexBuilder &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VertexBuilder &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);

        VulkanVertexShaderBuilder &build();

        Vertex buildVertex();
    };

    class VulkanVertexShaderBuilder {
    private:

        VulkanGraphicsEngineBuilder &mBuilder;

        std::vector<VertexBuilder> mVertexBuilders;

        std::vector<char> mCode;

        std::vector<Vertex> mVertices;

        uint32_t mCurrentBinding = -1;

    public:

        VulkanVertexShaderBuilder(VulkanGraphicsEngineBuilder &builder);

        ~VulkanVertexShaderBuilder();

        VulkanVertexShaderBuilder &code(std::vector<char> &&code);

        VulkanVertexShaderBuilder &vertex(std::function<void(VertexBuilder &)> configure);

        VertexBuilder &addVertex(uint32_t size);

        VertexBuilder &addVertex(uint32_t size, uint32_t binding);

        VulkanVertexShaderBuilder &addVertex(const Vertex &vertex);

        VulkanGraphicsEngineBuilder &build();
    };

} // engine
