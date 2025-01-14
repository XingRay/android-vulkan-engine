//
// Created by leixing on 2025/1/11.
//

#include "engine/VulkanVertexShaderBuilder.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanVertexShaderBuilder::VulkanVertexShaderBuilder(VulkanGraphicsEngineBuilder &builder) : mBuilder(builder) {

    }

    VulkanVertexShaderBuilder::~VulkanVertexShaderBuilder() = default;

    VulkanVertexShaderBuilder &VulkanVertexShaderBuilder::code(std::vector<char> &&code) {
        mCode = std::move(code);
        return *this;
    }

    VulkanVertexShaderBuilder &VulkanVertexShaderBuilder::vertex(std::function<void(VulkanVertexBuilder &)> configure) {
        VulkanVertexBuilder builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanVertexShaderBuilder &VulkanVertexShaderBuilder::addVertex(const VulkanVertex &vertex) {
        mVertices.push_back(vertex);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanVertexShaderBuilder::build() {
        mBuilder.setVertices(std::move(mVertices));
        mBuilder.setVertexShaderCode(std::move(mCode));
        return mBuilder;
    }


    VulkanVertexBuilder &VulkanVertexShaderBuilder::addVertex(uint32_t size) {
        return addVertex(size, mCurrentVertexBinding + 1);
    }

    VulkanVertexBuilder &VulkanVertexShaderBuilder::addVertex(uint32_t size, uint32_t binding) {
        mCurrentVertexBinding = binding;

        VulkanVertexBuilder builder = VulkanVertexBuilder{*this};
        builder.binding(binding).size(size);

        mVertexBuilders.push_back(builder);
        return mVertexBuilders.back();
    }

} // engine