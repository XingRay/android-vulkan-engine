//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanDescriptorBufferInfo.h"

namespace engine {

    class VulkanBufferDescriptorBinding {
    private:
        // buffer info
        std::unique_ptr<VulkanDescriptorBufferInfo> mVulkanDescriptorBufferInfo;

        //descriptor view
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

    public:
        VulkanBufferDescriptorBinding(vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange);

        ~VulkanBufferDescriptorBinding();

        [[nodiscard]]
        const std::unique_ptr<VulkanDescriptorBufferInfo> &getVulkanDescriptorBufferInfo() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorOffset() const;

        [[nodiscard]]
        uint32_t getDescriptorRange() const;

        VulkanBufferDescriptorBinding &setBufferInfo(std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanDescriptorBufferInfo);

//        [[nodiscard]]
//        vk::DescriptorBufferInfo createDescriptorBufferInfo() const;
    };

} // engine
