//
// Created by leixing on 2025/1/14.
//

#include "VertexBindingConfigure.h"
#include "vklite/VulkanUtil.h"

namespace vklite {

    VertexBindingConfigure::VertexBindingConfigure()
            : mStride(0), mAttributes({}), mVertexInputRate(vk::VertexInputRate::eVertex) {}

    VertexBindingConfigure::VertexBindingConfigure(VertexBindingConfigure &&other) noexcept
            : mStride(other.mStride), mAttributes(std::move(other.mAttributes)), mVertexInputRate(other.mVertexInputRate) {}

    VertexBindingConfigure::~VertexBindingConfigure() = default;

    uint32_t VertexBindingConfigure::getBinding() const {
        return mBinding;
    }

    // todo: calc stride by attributes size and align
    uint32_t VertexBindingConfigure::getStride() const {
        return mStride;
    }

    const std::unordered_map<uint32_t, VertexAttributeConfigure> &VertexBindingConfigure::getAttributes() const {
        return mAttributes;
    }

    VertexBindingConfigure &VertexBindingConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::stride(uint32_t stride) {
        mStride = stride;
        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset) {
        mCurrentAttributeLocation = location;
        mAttributes.emplace(location, VertexAttributeConfigure{format, offset});
        mCurrentAttributeOffset += VulkanUtil::getFormatSize(format);

        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, binding, ShaderFormatHelper::toVkFormat(format), offset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, mBinding, format, offset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, vk::Format format, uint32_t offset) {
        return addAttribute(location, mBinding, format, offset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, ShaderFormat format){
        return addAttribute(location, mBinding, format, mCurrentAttributeOffset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, vk::Format format){
        return addAttribute(location, mBinding, format, mCurrentAttributeOffset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(ShaderFormat format) {
        return addAttribute(mCurrentAttributeLocation + 1, mBinding, format, mCurrentAttributeOffset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(vk::Format format) {
        return addAttribute(mCurrentAttributeLocation + 1, mBinding, format, mCurrentAttributeOffset);
    }

} // engine