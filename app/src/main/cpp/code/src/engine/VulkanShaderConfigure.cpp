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

    // todo: move
    VulkanShaderConfigure &VulkanShaderConfigure::addVertex(const VulkanVertex &vertex) {
        mVertices.push_back(vertex);
        return *this;
    }

    VulkanShaderConfigure &VulkanShaderConfigure::uniformSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure) {
        VulkanDescriptorSetConfigure builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    // todo: move
    VulkanShaderConfigure &VulkanShaderConfigure::addUniformSet(const VulkanDescriptorSet& uniformSet) {
        mUniformSets.push_back(uniformSet);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanShaderConfigure::build() {
        // shader code
        mBuilder.setVertexShaderCode(std::move(mVertexShaderCode));
        mBuilder.setFragmentShaderCode(std::move(mFragmentShaderCode));

        // vertices
        mBuilder.setVertices(std::move(mVertices));

        // uniform / sampler / storage ...
        mBuilder.setUniformSets(std::move(mUniformSets));

        // push constant
        // todo


        return mBuilder;
    }

} // engine