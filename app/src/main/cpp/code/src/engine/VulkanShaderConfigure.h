//
// Created by leixing on 2025/1/11.
//

#pragma once

#include <functional>

#include "engine/ShaderFormat.h"
#include "engine/VulkanGraphicsEngineBuilder.h"
#include "engine/VulkanVertex.h"
#include "engine/VulkanVertexConfigure.h"
#include "engine/VulkanUniformSetConfigure.h"

namespace engine {

    class VulkanGraphicsEngineBuilder;

    class VulkanVertexConfigure;

    class VulkanUniformSetConfigure;

    class VulkanShaderConfigure {
    private:

        VulkanGraphicsEngineBuilder &mBuilder;

//        std::vector<VulkanVertexBuilder> mVertexBuilders;

        std::vector<char> mVertexShaderCode;

        std::vector<char> mFragmentShaderCode;

        std::vector<VulkanVertex> mVertices;

        uint32_t mCurrentVertexBinding = -1;

    public:

        VulkanShaderConfigure(VulkanGraphicsEngineBuilder &builder);

        ~VulkanShaderConfigure();

        VulkanShaderConfigure &vertexShaderCode(std::vector<char> &&code);

        VulkanShaderConfigure &fragmentShaderCode(std::vector<char> &&code);

        VulkanShaderConfigure &vertex(const std::function<void(VulkanVertexConfigure &)> &configure);

//        VulkanVertexBuilder &addVertex(uint32_t size);
//
//        VulkanVertexBuilder &addVertex(uint32_t size, uint32_t binding);

        VulkanShaderConfigure &addVertex(const VulkanVertex &vertex);

        VulkanShaderConfigure &uniformSet(const std::function<void(VulkanUniformSetConfigure &)> &configure);

        VulkanShaderConfigure &addUniformSet(uint32_t set, uint32_t binding, uint32_t size);

        VulkanGraphicsEngineBuilder &build();
    };

} // engine
