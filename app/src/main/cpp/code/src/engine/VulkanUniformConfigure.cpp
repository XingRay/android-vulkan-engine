//
// Created by leixing on 2025/3/24.
//

#include "engine/VulkanUniformConfigure.h"
#include "engine/VulkanDescriptorBindingConfigure.h"

namespace engine {

    VulkanUniformConfigure::VulkanUniformConfigure() = default;

    VulkanUniformConfigure::~VulkanUniformConfigure() = default;

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
        mVulkanBufferConfigure = std::make_unique<VulkanBufferConfigure>(capacity, data, size);

        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::setUniformBuffer(const std::shared_ptr<VulkanBufferView> &bufferView) {
        mVulkanBufferConfigure = std::make_unique<VulkanBufferConfigure>(bufferView);

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

//    std::shared_ptr<VulkanDeviceLocalUniformBuffer> VulkanUniformConfigure::createUniformBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
//        if (mBuffer != nullptr) {
//            return mBuffer;
//        }
//
//        if (mCreateBufferCapacity == 0) {
//            return nullptr;
//        }
//
//        std::shared_ptr<VulkanDeviceLocalUniformBuffer> buffer = std::make_shared<VulkanDeviceLocalUniformBuffer>(vulkanDevice, mCreateBufferCapacity);
//        if (!mCreateBufferData.empty()) {
//            buffer->update(commandPool, mCreateBufferData);
//        }
//
//        return buffer;
//    }

    std::unique_ptr<VulkanDescriptorBindingConfigure> VulkanUniformConfigure::createVulkanDescriptorSetConfigure() {
        std::shared_ptr<VulkanBufferInterface> buffer = createUniformBuffer();
        std::unique_ptr<VulkanBufferView> bufferView = std::make_unique<VulkanBufferView>();
        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eUniformBuffer,
                                                                  mDescriptorCount, mShaderStageFlags, );
    }

} // engine