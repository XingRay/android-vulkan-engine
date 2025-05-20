//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/configure/descriptor/VulkanDescriptorBufferInfoConfigure.h"

namespace vklite {

    class VulkanDescriptorBindingConfigure;
    class VulkanDescriptorSetConfigure;

    class VulkanUniformConfigure {
        friend VulkanDescriptorSetConfigure;

    private:
        uint32_t mBinding;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<VulkanDescriptorBufferInfoConfigure> mVulkanDescriptorBufferInfoConfigure;

    public:
        VulkanUniformConfigure();

        ~VulkanUniformConfigure();

        VulkanUniformConfigure &binding(uint32_t binding);

        VulkanUniformConfigure &descriptorOffset(uint32_t offset);

        VulkanUniformConfigure &descriptorRange(uint32_t range);

        VulkanUniformConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        VulkanUniformConfigure &setUniformBuffer(uint32_t capacity, const void *data, uint32_t size);

        VulkanUniformConfigure &setUniformBuffer(std::unique_ptr<BufferInfo> &&bufferInfo);

        template<class T>
        VulkanUniformConfigure &setUniformBuffer(uint32_t capacity, const std::vector<T> &data) {
            return setUniformBuffer(capacity, data.data(), data.size() * sizeof(T));
        }

        template<class T>
        VulkanUniformConfigure &setUniformBuffer(const std::vector<T> &data) {
            uint32_t size = data.size() * sizeof(T);
            return setUniformBuffer(size, data.data(), size);
        }

        template<class T>
        VulkanUniformConfigure &setUniformBuffer(const T &data) {
            uint32_t size = sizeof(T);
            return setUniformBuffer(size, &data, size);
        }

    private:

        [[nodiscard]]
        std::unique_ptr<VulkanDescriptorBindingConfigure> createVulkanDescriptorBindingConfigure();
    };

} // engine
