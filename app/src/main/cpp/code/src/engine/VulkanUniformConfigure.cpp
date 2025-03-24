//
// Created by leixing on 2025/3/24.
//

#include "VulkanUniformConfigure.h"

namespace engine {
    VulkanUniformConfigure::VulkanUniformConfigure() {

    }

    VulkanUniformConfigure::~VulkanUniformConfigure() {

    }

    VulkanUniformConfigure &VulkanUniformConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::descriptorCount(uint32_t count) {
        mDescriptorCount = count;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::setUniformBuffer(uint32_t capacity, const void *data, uint32_t size) {
        mCreateBufferCapacity = capacity;

        // 预分配内存
        mCreateBufferData.reserve(size);
        const uint8_t *byteData = static_cast<const uint8_t *>(data);
        // 深拷贝数据
        mCreateBufferData.assign(byteData, byteData + size);

        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::setUniformBuffer(const std::shared_ptr<VulkanDeviceLocalUniformBuffer> &buffer) {
        mBuffer = buffer;
        return *this;
    }

    vk::DescriptorSetLayoutBinding VulkanUniformConfigure::createDescriptorSetLayoutBinding() const {
        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};

        descriptorSetLayoutBinding
                .setBinding(mBinding)
                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(mDescriptorCount)
                .setStageFlags(mShaderStageFlags);

        return descriptorSetLayoutBinding;
    }

    std::shared_ptr<VulkanDeviceLocalUniformBuffer> VulkanUniformConfigure::createUniformBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
        if (mBuffer != nullptr) {
            return mBuffer;
        }

        if (mCreateBufferCapacity == 0) {
            return nullptr;
        }

        std::shared_ptr<VulkanDeviceLocalUniformBuffer> buffer = std::make_shared<VulkanDeviceLocalUniformBuffer>(vulkanDevice, mCreateBufferCapacity);
        if (!mCreateBufferData.empty()) {
            buffer->update(commandPool, mCreateBufferData);
        }

        return buffer;
    }

} // engine