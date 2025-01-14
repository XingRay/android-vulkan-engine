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

    VulkanVertexShaderBuilder &VulkanVertexShaderBuilder::vertex(std::function<void(VertexBuilder &)> configure) {
        VertexBuilder builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanVertexShaderBuilder &VulkanVertexShaderBuilder::addVertex(const Vertex &vertex) {
        mVertices.push_back(vertex);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanVertexShaderBuilder::build() {
        mBuilder.setVertices(std::move(mVertices));
        mBuilder.setVertexShaderCode(std::move(mCode));
        return mBuilder;
    }


    VertexBuilder &VulkanVertexShaderBuilder::addVertex(uint32_t size) {
        return addVertex(size, mCurrentBinding + 1);
    }

    VertexBuilder &VulkanVertexShaderBuilder::addVertex(uint32_t size, uint32_t binding) {
        mCurrentBinding = binding;

        VertexBuilder builder = VertexBuilder{*this};
        builder.binding(binding).size(size);

        mVertexBuilders.push_back(builder);
        return mVertexBuilders.back();
    }

    VertexBuilder::VertexBuilder(VulkanVertexShaderBuilder &builder)
            : mBuilder(builder) {

    }

    VertexBuilder &VertexBuilder::size(uint32_t size) {
        mSize = size;
        return *this;
    }

    VertexBuilder &VertexBuilder::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VertexBuilder &VertexBuilder::addAttribute(ShaderFormat format) {
        return addAttribute(ShaderFormatHelper::toVkFormat(format));
    }

    VertexBuilder &VertexBuilder::addAttribute(vk::Format format) {
        addAttribute(mCurrentLocation + 1, mBinding, format, mCurrentOffset);
        return *this;
    }

    VertexBuilder &VertexBuilder::addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, binding, ShaderFormatHelper::toVkFormat(format), offset);
    }

    VertexBuilder &VertexBuilder::addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset) {
        mCurrentLocation = location;

        mAttributes.push_back(VertexAttribute{binding, location, format, offset});

        mCurrentOffset += VulkanUtil::getFormatSize(format);

        return *this;
    }

    Vertex VertexBuilder::buildVertex() {
        return Vertex{mBinding, mSize, mAttributes};
    }

    VulkanVertexShaderBuilder &VertexBuilder::build() {
        mBuilder.addVertex(buildVertex());
        return mBuilder;
    }

} // engine