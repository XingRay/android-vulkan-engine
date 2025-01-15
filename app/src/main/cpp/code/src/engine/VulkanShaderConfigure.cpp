//
// Created by leixing on 2025/1/11.
//

#include "engine/VulkanShaderConfigure.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanShaderConfigure::VulkanShaderConfigure(VulkanGraphicsEngineBuilder &builder) : mBuilder(builder) {

    }

    VulkanShaderConfigure::~VulkanShaderConfigure() = default;

    VulkanShaderConfigure &VulkanShaderConfigure::vertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    VulkanShaderConfigure &VulkanShaderConfigure::fragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    VulkanShaderConfigure &VulkanShaderConfigure::vertex(const std::function<void(VulkanVertexConfigure &)> &configure) {
        VulkanVertexConfigure builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

//    VulkanVertexBuilder &VulkanShaderBuilder::addVertex(uint32_t size) {
//        return addVertex(size, mCurrentVertexBinding + 1);
//    }
//
//    VulkanVertexBuilder &VulkanShaderBuilder::addVertex(uint32_t size, uint32_t binding) {
//        mCurrentVertexBinding = binding;
//
//        VulkanVertexBuilder builder = VulkanVertexBuilder{*this};
//        builder.binding(binding).size(size);
//
//        mVertexBuilders.push_back(builder);
//        return mVertexBuilders.back();
//    }

    VulkanShaderConfigure &VulkanShaderConfigure::addVertex(const VulkanVertex &vertex) {
        mVertices.push_back(vertex);
        return *this;
    }

    VulkanShaderConfigure &VulkanShaderConfigure::uniformSet(const std::function<void(VulkanUniformSetConfigure &)> &configure) {
        VulkanUniformSetConfigure builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanShaderConfigure &VulkanShaderConfigure::addUniformSet(uint32_t set, uint32_t binding, uint32_t size) {

        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanShaderConfigure::build() {
        mBuilder.setVertexShaderCode(std::move(mVertexShaderCode));
        mBuilder.setFragmentShaderCode(std::move(mFragmentShaderCode));

        mBuilder.setVertices(std::move(mVertices));
//        mBuilder.setUniformSets();

        return mBuilder;
    }

} // engine