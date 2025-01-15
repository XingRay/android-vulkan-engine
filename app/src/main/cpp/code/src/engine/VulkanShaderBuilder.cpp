//
// Created by leixing on 2025/1/11.
//

#include "engine/VulkanShaderBuilder.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanShaderBuilder::VulkanShaderBuilder(VulkanGraphicsEngineBuilder &builder) : mBuilder(builder) {

    }

    VulkanShaderBuilder::~VulkanShaderBuilder() = default;

    VulkanShaderBuilder &VulkanShaderBuilder::vertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    VulkanShaderBuilder &VulkanShaderBuilder::fragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    VulkanShaderBuilder &VulkanShaderBuilder::vertex(std::function<void(VulkanVertexBuilder &)> configure) {
        VulkanVertexBuilder builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanVertexBuilder &VulkanShaderBuilder::addVertex(uint32_t size) {
        return addVertex(size, mCurrentVertexBinding + 1);
    }

    VulkanVertexBuilder &VulkanShaderBuilder::addVertex(uint32_t size, uint32_t binding) {
        mCurrentVertexBinding = binding;

        VulkanVertexBuilder builder = VulkanVertexBuilder{*this};
        builder.binding(binding).size(size);

        mVertexBuilders.push_back(builder);
        return mVertexBuilders.back();
    }

    VulkanShaderBuilder &VulkanShaderBuilder::addVertex(const VulkanVertex &vertex) {
        mVertices.push_back(vertex);
        return *this;
    }

    VulkanShaderBuilder &VulkanShaderBuilder::uniformSet(std::function<void(VulkanUniformSetBuilder &)> configure) {
        VulkanUniformSetBuilder builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanShaderBuilder &VulkanShaderBuilder::addUniformSet(uint32_t set, uint32_t binding, uint32_t size) {

        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanShaderBuilder::build() {
        mBuilder.setVertexShaderCode(std::move(mVertexShaderCode));
        mBuilder.setFragmentShaderCode(std::move(mFragmentShaderCode));
        mBuilder.setVertices(std::move(mVertices));
        return mBuilder;
    }

} // engine