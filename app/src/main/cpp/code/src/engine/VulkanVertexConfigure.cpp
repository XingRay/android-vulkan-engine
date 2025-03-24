//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanVertexConfigure.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanVertexConfigure::VulkanVertexConfigure()
            : mStride(0), mBinding(0), mAttributes({}), mCreateBufferCapacity(0) {

    }

    VulkanVertexConfigure::~VulkanVertexConfigure() = default;

    uint32_t VulkanVertexConfigure::getBinding() const {
        return mBinding;
    }

    uint32_t VulkanVertexConfigure::getStride() const {
        return mStride;
    }

    const std::vector<VulkanVertexAttributeConfigure> &VulkanVertexConfigure::getAttributes() const {
        return mAttributes;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::stride(uint32_t stride) {
        mStride = stride;
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
        mAttributes.push_back(VulkanVertexAttributeConfigure{binding, location, format, offset});
        mCurrentAttributeOffset += VulkanUtil::getFormatSize(format);

        return *this;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::setVertexBuffer(uint32_t capacity) {
        mCreateBufferCapacity = capacity;
        return *this;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::setVertexBuffer(uint32_t capacity, const void *data, uint32_t size) {
        mCreateBufferCapacity = capacity;

        // 预分配内存
        mCreateBufferData.reserve(size);
        const uint8_t *byteData = static_cast<const uint8_t *>(data);
        // 深拷贝数据
        mCreateBufferData.assign(byteData, byteData + size);

        return *this;
    }

    VulkanVertexConfigure &VulkanVertexConfigure::setVertexBuffer(const void *data, uint32_t size) {
        return setVertexBuffer(size, data, size);
    }

    VulkanVertexConfigure &VulkanVertexConfigure::setVertexBuffer(const std::shared_ptr<VulkanDeviceLocalVertexBuffer> &buffer) {
        mBuffer = buffer;
        return *this;
    }

    std::shared_ptr<VulkanDeviceLocalVertexBuffer> VulkanVertexConfigure::createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
        if (mBuffer != nullptr) {
            return mBuffer;
        }

        if (mCreateBufferCapacity == 0) {
            return nullptr;
        }

        std::shared_ptr<VulkanDeviceLocalVertexBuffer> buffer = std::make_shared<VulkanDeviceLocalVertexBuffer>(vulkanDevice, mCreateBufferCapacity);
        if (!mCreateBufferData.empty()) {
            buffer->update(commandPool, mCreateBufferData);
        }

        return buffer;
    }

} // engine