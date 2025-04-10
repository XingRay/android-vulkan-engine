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

    class VulkanVertexConfigure {
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
        VulkanVertexConfigure();

        ~VulkanVertexConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getStride() const;

        [[nodiscard]]
        const std::vector<VulkanVertexAttributeConfigure> &getAttributes() const;

        VulkanVertexConfigure &stride(uint32_t stride);

        VulkanVertexConfigure &binding(uint32_t binding);

        VulkanVertexConfigure &addAttribute(ShaderFormat format);

        VulkanVertexConfigure &addAttribute(vk::Format format);

        VulkanVertexConfigure &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VulkanVertexConfigure &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);


        VulkanVertexConfigure &setVertexBuffer(uint32_t capacity);

        VulkanVertexConfigure &setVertexBuffer(uint32_t capacity, const void *data, uint32_t size);

        VulkanVertexConfigure &setVertexBuffer(const void *data, uint32_t size);

        template<class T>
        VulkanVertexConfigure &setVertexBuffer(uint32_t capacity, const std::vector<T> &data) {
            return setVertexBuffer(capacity, data.data(), data.size() * sizeof(T));
        }

        template<class T>
        VulkanVertexConfigure &setVertexBuffer(const std::vector<T> &data) {
            uint32_t size = data.size() * sizeof(T);
            return setVertexBuffer(size, data.data(), size);
        }

        VulkanVertexConfigure &setVertexBuffer(const std::shared_ptr<VulkanDeviceLocalVertexBuffer> &buffer);

        [[nodiscard]]
        std::shared_ptr<VulkanDeviceLocalVertexBuffer> createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
