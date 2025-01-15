//
// Created by leixing on 2025/1/11.
//

#pragma once

#include <functional>

#include "engine/ShaderFormat.h"
#include "engine/VulkanGraphicsEngineBuilder.h"
#include "engine/VulkanVertex.h"
#include "engine/VulkanVertexBuilder.h"
#include "engine/VulkanUniformSetBuilder.h"

namespace engine {

    class VulkanGraphicsEngineBuilder;
    class VulkanVertexBuilder;
    class VulkanUniformSetBuilder;

    class VulkanShaderBuilder {
    private:

        VulkanGraphicsEngineBuilder &mBuilder;

        std::vector<VulkanVertexBuilder> mVertexBuilders;

        std::vector<char> mVertexShaderCode;

        std::vector<char> mFragmentShaderCode;

        std::vector<VulkanVertex> mVertices;

        uint32_t mCurrentVertexBinding = -1;

    public:

        VulkanShaderBuilder(VulkanGraphicsEngineBuilder &builder);

        ~VulkanShaderBuilder();

        VulkanShaderBuilder &vertexShaderCode(std::vector<char> &&code);

        VulkanShaderBuilder &fragmentShaderCode(std::vector<char> &&code);

        VulkanShaderBuilder &vertex(std::function<void(VulkanVertexBuilder &)> configure);

        VulkanVertexBuilder &addVertex(uint32_t size);

        VulkanVertexBuilder &addVertex(uint32_t size, uint32_t binding);

        VulkanShaderBuilder &addVertex(const VulkanVertex &vertex);

        VulkanShaderBuilder &uniformSet(std::function<void(VulkanUniformSetBuilder&)> configure);

        VulkanShaderBuilder &addUniformSet(uint32_t set, uint32_t binding, uint32_t size);

        VulkanGraphicsEngineBuilder &build();
    };

} // engine
