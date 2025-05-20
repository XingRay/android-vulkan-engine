//
// Created by leixing on 2025/4/26.
//

#include "VertexBufferBindingConfigure.h"

namespace vklite {

    VertexBufferBindingConfigure::VertexBufferBindingConfigure() {

    }

    VertexBufferBindingConfigure::~VertexBufferBindingConfigure() {

    }

    //    VertexBindingConfigure &VertexBindingConfigure::setVertexBuffer(uint32_t capacity) {
//        mCreateBufferCapacity = capacity;
//        return *this;
//    }
//
//    VertexBindingConfigure &VertexBindingConfigure::setVertexBuffer(uint32_t capacity, const void *data, uint32_t size) {
//        mCreateBufferCapacity = capacity;
//
//        // 预分配内存
//        mCreateBufferData.reserve(size);
//        const uint8_t *byteData = static_cast<const uint8_t *>(data);
//        // 深拷贝数据
//        mCreateBufferData.assign(byteData, byteData + size);
//
//        return *this;
//    }
//
//    VertexBindingConfigure &VertexBindingConfigure::setVertexBuffer(const void *data, uint32_t size) {
//        return setVertexBuffer(size, data, size);
//    }
//
//    VertexBindingConfigure &VertexBindingConfigure::setVertexBuffer(const std::shared_ptr<VulkanDeviceLocalVertexBuffer> &buffer) {
//        mBuffer = buffer;
//        return *this;
//    }
//
//    std::shared_ptr<VulkanDeviceLocalVertexBuffer> VertexBindingConfigure::createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
//        if (mBuffer != nullptr) {
//            return mBuffer;
//        }
//
//        if (mCreateBufferCapacity == 0) {
//            return nullptr;
//        }
//
//        std::shared_ptr<VulkanDeviceLocalVertexBuffer> buffer = std::make_shared<VulkanDeviceLocalVertexBuffer>(vulkanDevice, mCreateBufferCapacity);
//        if (!mCreateBufferData.empty()) {
//            buffer->update(commandPool, mCreateBufferData);
//        }
//
//        return buffer;
//    }

} // engine