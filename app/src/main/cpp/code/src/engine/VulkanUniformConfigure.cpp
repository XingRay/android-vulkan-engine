//
// Created by leixing on 2025/3/24.
//

#include "engine/VulkanUniformConfigure.h"
#include "engine/VulkanDescriptorBindingConfigure.h"

namespace engine {

    VulkanUniformConfigure::VulkanUniformConfigure()
            : mBinding(0), mDescriptorOffset(0), mDescriptorRange(1) {}

    VulkanUniformConfigure::~VulkanUniformConfigure() = default;

    VulkanUniformConfigure &VulkanUniformConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::descriptorOffset(uint32_t offset) {
        mDescriptorOffset = offset;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::descriptorRange(uint32_t range) {
        mDescriptorRange = range;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::setUniformBuffer(uint32_t capacity, const void *data, uint32_t size) {
        mVulkanBufferViewConfigure = std::make_unique<VulkanBufferViewConfigure>(capacity, data, size);
        return *this;
    }

    VulkanUniformConfigure &VulkanUniformConfigure::setUniformBuffer(std::unique_ptr<VulkanBufferView> &&bufferView) {
        mVulkanBufferViewConfigure = std::make_unique<VulkanBufferViewConfigure>(std::move(bufferView));
        return *this;
    }

//    vk::DescriptorSetLayoutBinding VulkanUniformConfigure::createDescriptorSetLayoutBinding() const {
//        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};
//
//        descriptorSetLayoutBinding
//                .setBinding(mBinding)
//                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
//                .setDescriptorCount(mDescriptorCount)
//                .setStageFlags(mShaderStageFlags);
//
//        return descriptorSetLayoutBinding;
//    }

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

//    std::unique_ptr<VulkanDescriptorBindingConfigure> VulkanUniformConfigure::createVulkanDescriptorSetConfigure() {
//        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eUniformBuffer,
//                                                                  mDescriptorCount, mShaderStageFlags, );
//    }

    std::unique_ptr<VulkanDescriptorBindingConfigure> VulkanUniformConfigure::createVulkanDescriptorBindingConfigure() {
        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eUniformBuffer, mDescriptorOffset, mDescriptorRange,
                                                                  mShaderStageFlags, std::move(mVulkanBufferViewConfigure));
    }

} // engine