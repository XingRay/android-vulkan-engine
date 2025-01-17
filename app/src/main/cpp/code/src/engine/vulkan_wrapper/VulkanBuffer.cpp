//
// Created by leixing on 2025/1/16.
//

#include "VulkanBuffer.h"

namespace engine {

    VulkanBuffer::VulkanBuffer(VulkanBufferType type) : mType(type), mIndex(0), mBinding(0) {

    }

    engine::VulkanBuffer::~VulkanBuffer() = default;


    VulkanBufferType VulkanBuffer::getType() const {
        return mType;
    }

    uint32_t VulkanBuffer::getBinding() const {
        return mBinding;
    }

    uint32_t VulkanBuffer::getIndex() const {
        return mIndex;
    }

}