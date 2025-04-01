//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "engine/VulkanBufferViewConfigure.h"

namespace engine {

    class VulkanDescriptorBindingConfigure;

    class VulkanUniformConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<VulkanBufferConfigure> mVulkanBufferConfigure;

    public:
        VulkanUniformConfigure();

        ~VulkanUniformConfigure();

        VulkanUniformConfigure &binding(uint32_t binding);

        VulkanUniformConfigure &descriptorCount(uint32_t count);

        VulkanUniformConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        VulkanUniformConfigure &setUniformBuffer(uint32_t capacity, const void *data, uint32_t size);

        VulkanUniformConfigure &setUniformBuffer(const std::shared_ptr<VulkanBufferView> &bufferView);

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

        [[nodiscard]]
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding() const;

        [[nodiscard]]
        std::shared_ptr<VulkanDeviceLocalUniformBuffer> createUniformBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;

        [[nodiscard]]
        std::unique_ptr<VulkanDescriptorBindingConfigure> createVulkanDescriptorSetConfigure();
    };

} // engine
