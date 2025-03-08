//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanVertexConfigure.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanVertexConfigure::VulkanVertexConfigure()
            : mSize(0), mBinding(0), mAttributes({}) {

    }

    VulkanVertexConfigure &VulkanVertexConfigure::size(uint32_t size) {
        mSize = size;
        return *this;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::addAttribute(ShaderFormat format) {
        return addAttribute(ShaderFormatHelper::toVkFormat(format));
    }

    VulkanVertexConfigure &VulkanVertexConfigure::addAttribute(vk::Format format) {
        addAttribute(mCurrentAttributeLocation + 1, mBinding, format, mCurrentAttributeOffset);
        return *this;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, binding, ShaderFormatHelper::toVkFormat(format), offset);
    }

    VulkanVertexConfigure &VulkanVertexConfigure::addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset) {
        mCurrentAttributeLocation = location;

        mAttributes.push_back(VulkanVertexAttribute{binding, location, format, offset});

        mCurrentAttributeOffset += VulkanUtil::getFormatSize(format);

        return *this;
    }

    VulkanVertex VulkanVertexConfigure::build() const {
        return VulkanVertex{mBinding, mSize, mAttributes};
    }

} // engine