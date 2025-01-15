//
// Created by leixing on 2025/1/14.
//

#pragma once

#include "engine/VulkanShaderBuilder.h"

namespace engine {

    class VulkanShaderBuilder;

    class VulkanVertexBuilder {
    private:
        VulkanShaderBuilder &mBuilder;

        uint32_t mSize;
        uint32_t mBinding;
        std::vector<VulkanVertexAttribute> mAttributes;

        uint32_t mCurrentLocation = -1;
        uint32_t mCurrentOffset = 0;

    public:
        VulkanVertexBuilder(VulkanShaderBuilder &builder);

        VulkanVertexBuilder &size(uint32_t size);

        VulkanVertexBuilder &binding(uint32_t binding);

        VulkanVertexBuilder &addAttribute(ShaderFormat format);

        VulkanVertexBuilder &addAttribute(vk::Format format);

        VulkanVertexBuilder &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VulkanVertexBuilder &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);

        VulkanShaderBuilder &build();

        VulkanVertex buildVertex();
    };

} // engine
