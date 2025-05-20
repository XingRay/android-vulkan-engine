//
// Created by leixing on 2025/4/26.
//

#pragma once

namespace vklite {

    class VertexBufferBindingConfigure {
    private:

        //        // set buffer
//        std::shared_ptr<VulkanDeviceLocalVertexBuffer> mBuffer;
//
//        // create buffer
//        uint32_t mCreateBufferCapacity;
//        std::vector<uint8_t> mCreateBufferData;

    public:
        VertexBufferBindingConfigure();

        ~VertexBufferBindingConfigure();

        //        VertexBindingConfigure &setVertexBuffer(uint32_t capacity);
//
//        VertexBindingConfigure &setVertexBuffer(uint32_t capacity, const void *data, uint32_t size);
//
//        VertexBindingConfigure &setVertexBuffer(const void *data, uint32_t size);
//
//        template<class T>
//        VertexBindingConfigure &setVertexBuffer(uint32_t capacity, const std::vector<T> &data) {
//            return setVertexBuffer(capacity, data.data(), data.size() * sizeof(T));
//        }
//
//        template<class T>
//        VertexBindingConfigure &setVertexBuffer(const std::vector<T> &data) {
//            uint32_t size = data.size() * sizeof(T);
//            return setVertexBuffer(size, data.data(), size);
//        }
//
//        VertexBindingConfigure &setVertexBuffer(const std::shared_ptr<VulkanDeviceLocalVertexBuffer> &buffer);
//
//        [[nodiscard]]
//        std::shared_ptr<VulkanDeviceLocalVertexBuffer> createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
