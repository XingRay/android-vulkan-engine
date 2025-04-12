//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/buffer/VulkanDescriptorBufferInfo.h"
#include "engine/vulkan_wrapper/image/VulkanDescriptorImageInfo.h"

namespace engine {

    class VulkanDescriptorBinding {
    private:
        // descriptor info
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

        // buffer info
        std::unique_ptr<VulkanDescriptorBufferInfo> mVulkanDescriptorBufferInfo;

        // image info
        std::unique_ptr<VulkanDescriptorImageInfo> mVulkanDescriptorImageInfo;

    public:
        VulkanDescriptorBinding(vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange);

        ~VulkanDescriptorBinding();

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorOffset() const;

        [[nodiscard]]
        uint32_t getDescriptorRange() const;

        [[nodiscard]]
        const std::unique_ptr<VulkanDescriptorBufferInfo> &getVulkanDescriptorBufferInfo() const;

        VulkanDescriptorBinding &setVulkanDescriptorBufferInfo(std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanDescriptorBufferInfo);

        [[nodiscard]]
        const std::unique_ptr<VulkanDescriptorImageInfo> &getVulkanDescriptorImageInfo() const;

        VulkanDescriptorBinding &setVulkanDescriptorImageInfo(std::unique_ptr<VulkanDescriptorImageInfo> &&vulkanDescriptorImageInfo);

        VulkanDescriptorBinding &updateUniformBuffer(const VulkanCommandPool& commandPool, const void *data, uint32_t size);
    };

} // engine
