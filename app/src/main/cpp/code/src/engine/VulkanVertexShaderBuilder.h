//
// Created by leixing on 2025/1/11.
//

#pragma once

#include <functional>

#include "engine/ShaderFormat.h"
#include "engine/VulkanGraphicsEngineBuilder.h"
#include "engine/VulkanVertex.h"
#include "engine/VulkanVertexBuilder.h"

namespace engine {

    class VulkanGraphicsEngineBuilder;
    class VulkanVertexBuilder;

    class VulkanVertexShaderBuilder {
    private:

        VulkanGraphicsEngineBuilder &mBuilder;

        std::vector<VulkanVertexBuilder> mVertexBuilders;

        std::vector<char> mCode;

        std::vector<VulkanVertex> mVertices;

        uint32_t mCurrentVertexBinding = -1;

    public:

        VulkanVertexShaderBuilder(VulkanGraphicsEngineBuilder &builder);

        ~VulkanVertexShaderBuilder();

        VulkanVertexShaderBuilder &code(std::vector<char> &&code);

        VulkanVertexShaderBuilder &vertex(std::function<void(VulkanVertexBuilder &)> configure);

        VulkanVertexBuilder &addVertex(uint32_t size);

        VulkanVertexBuilder &addVertex(uint32_t size, uint32_t binding);

        VulkanVertexShaderBuilder &addVertex(const VulkanVertex &vertex);

//        VulkanVertexShaderBuilder &addUniform();

        VulkanGraphicsEngineBuilder &build();
    };

} // engine
