//
// Created by leixing on 2025/3/11.
//
#include "engine/VulkanVertexAttributeConfigure.h"

namespace engine {
    VulkanVertexAttributeConfigure::VulkanVertexAttributeConfigure(uint32_t binding, uint32_t location, vk::Format format, uint32_t offset)
            : mBinding(binding), mLocation(location), mFormat(format), mOffset(offset) {

    }

    VulkanVertexAttributeConfigure::~VulkanVertexAttributeConfigure() = default;

    uint32_t VulkanVertexAttributeConfigure::getBinding() const {
        return mBinding;
    }

    uint32_t VulkanVertexAttributeConfigure::getLocation() const {
        return mLocation;
    }

    vk::Format VulkanVertexAttributeConfigure::getFormat() const {
        return mFormat;
    }

    uint32_t VulkanVertexAttributeConfigure::getOffset() const {
        return mOffset;
    }
}