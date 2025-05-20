//
// Created by leixing on 2025/1/14.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "VertexAttributeConfigure.h"
#include "vklite/ShaderFormat.h"

namespace vklite {

    class VertexBindingConfigure {
    private:
        uint32_t mBinding;
        uint32_t mStride;
        vk::VertexInputRate mVertexInputRate;

        // location -> VertexAttributeConfigure
        std::unordered_map<uint32_t, VertexAttributeConfigure> mAttributes;

        uint32_t mCurrentAttributeLocation = -1;
        uint32_t mCurrentAttributeOffset = 0;

    public:
        VertexBindingConfigure();

        VertexBindingConfigure(VertexBindingConfigure &&other)noexcept;

        ~VertexBindingConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getStride() const;

        [[nodiscard]]
        const std::unordered_map<uint32_t, VertexAttributeConfigure> &getAttributes() const;

        VertexBindingConfigure &stride(uint32_t stride);

        VertexBindingConfigure &binding(uint32_t binding);

        VertexBindingConfigure &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, ShaderFormat format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, vk::Format format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, ShaderFormat format);

        VertexBindingConfigure &addAttribute(uint32_t location, vk::Format format);

        VertexBindingConfigure &addAttribute(ShaderFormat format);

        VertexBindingConfigure &addAttribute(vk::Format format);

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
