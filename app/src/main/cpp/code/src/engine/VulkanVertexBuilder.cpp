//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanVertexBuilder.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanVertexBuilder::VulkanVertexBuilder(VulkanShaderBuilder &builder)
            : mBuilder(builder), mSize(0), mBinding(0), mAttributes({}) {

    }

    VulkanVertexBuilder &VulkanVertexBuilder::size(uint32_t size) {
        mSize = size;
        return *this;
    }

    VulkanVertexBuilder &VulkanVertexBuilder::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VulkanVertexBuilder &VulkanVertexBuilder::addAttribute(ShaderFormat format) {
        return addAttribute(ShaderFormatHelper::toVkFormat(format));
    }

    VulkanVertexBuilder &VulkanVertexBuilder::addAttribute(vk::Format format) {
        addAttribute(mCurrentLocation + 1, mBinding, format, mCurrentOffset);
        return *this;
    }

    VulkanVertexBuilder &VulkanVertexBuilder::addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, binding, ShaderFormatHelper::toVkFormat(format), offset);
    }

    VulkanVertexBuilder &VulkanVertexBuilder::addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset) {
        mCurrentLocation = location;

        mAttributes.push_back(VulkanVertexAttribute{binding, location, format, offset});

        mCurrentOffset += VulkanUtil::getFormatSize(format);

        return *this;
    }

    VulkanVertex VulkanVertexBuilder::buildVertex() {
        return VulkanVertex{mBinding, mSize, mAttributes};
    }

    VulkanShaderBuilder &VulkanVertexBuilder::build() {
        mBuilder.addVertex(buildVertex());
        return mBuilder;
    }

} // engine