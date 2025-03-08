//
// Created by leixing on 2025/1/14.
//

#pragma once

#include <cstdint>
#include <memory>

#include "engine/VulkanVertexAttribute.h"
#include "engine/VulkanVertex.h"
#include "engine/ShaderFormat.h"


namespace engine {

    class VulkanShaderConfigure;

    class VulkanVertexConfigure {
    private:
        uint32_t mBinding;
        uint32_t mSize;

        std::vector<VulkanVertexAttribute> mAttributes;

        uint32_t mCurrentAttributeLocation = -1;
        uint32_t mCurrentAttributeOffset = 0;

    public:
        VulkanVertexConfigure();

        VulkanVertexConfigure &size(uint32_t size);

        VulkanVertexConfigure &binding(uint32_t binding);

        VulkanVertexConfigure &addAttribute(ShaderFormat format);

        VulkanVertexConfigure &addAttribute(vk::Format format);

        VulkanVertexConfigure &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VulkanVertexConfigure &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);

        VulkanVertex build() const;
    };

} // engine
