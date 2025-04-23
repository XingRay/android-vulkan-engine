//
// Created by leixing on 2025/1/14.
//

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "VulkanVertexAttributeConfigure.h"
#include "engine/ShaderFormat.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalVertexBuffer.h"

namespace engine {

    class VertexConfigure {
    private:
        uint32_t mBinding;
        uint32_t mStride;

        std::vector<VulkanVertexAttributeConfigure> mAttributes;

        // set buffer
        std::shared_ptr<VulkanDeviceLocalVertexBuffer> mBuffer;

        // create buffer
        uint32_t mCreateBufferCapacity;
        std::vector<uint8_t> mCreateBufferData;

        uint32_t mCurrentAttributeLocation = -1;
        uint32_t mCurrentAttributeOffset = 0;

    public:
        VertexConfigure();

        ~VertexConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getStride() const;

        [[nodiscard]]
        const std::vector<VulkanVertexAttributeConfigure> &getAttributes() const;

        VertexConfigure &stride(uint32_t stride);

        VertexConfigure &binding(uint32_t binding);

        VertexConfigure &addAttribute(ShaderFormat format);

        VertexConfigure &addAttribute(vk::Format format);

        VertexConfigure &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VertexConfigure &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);


        VertexConfigure &setVertexBuffer(uint32_t capacity);

        VertexConfigure &setVertexBuffer(uint32_t capacity, const void *data, uint32_t size);

        VertexConfigure &setVertexBuffer(const void *data, uint32_t size);

        template<class T>
        VertexConfigure &setVertexBuffer(uint32_t capacity, const std::vector<T> &data) {
            return setVertexBuffer(capacity, data.data(), data.size() * sizeof(T));
        }

        template<class T>
        VertexConfigure &setVertexBuffer(const std::vector<T> &data) {
            uint32_t size = data.size() * sizeof(T);
            return setVertexBuffer(size, data.data(), size);
        }

        VertexConfigure &setVertexBuffer(const std::shared_ptr<VulkanDeviceLocalVertexBuffer> &buffer);

        [[nodiscard]]
        std::shared_ptr<VulkanDeviceLocalVertexBuffer> createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
