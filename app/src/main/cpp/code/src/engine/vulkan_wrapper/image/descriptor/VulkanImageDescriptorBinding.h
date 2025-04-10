//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/image/descriptor/VulkanDescriptorImageInfo.h"

namespace engine {

    class VulkanImageDescriptorBinding {
    private:
        std::unique_ptr<VulkanDescriptorImageInfo> mVulkanDescriptorImageInfo;

        //descriptor view
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

    public:
        VulkanImageDescriptorBinding(vk::DescriptorType descriptorType,
                                     uint32_t descriptorOffset,
                                     uint32_t descriptorRange);

        ~VulkanImageDescriptorBinding();

        [[nodiscard]]
        const std::unique_ptr<VulkanDescriptorImageInfo> &getVulkanDescriptorImageInfo() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorOffset() const;

        [[nodiscard]]
        uint32_t getDescriptorRange() const;

        VulkanImageDescriptorBinding &setVulkanDescriptorImageInfo(std::unique_ptr<VulkanDescriptorImageInfo> &&imageInfo);
    };

} // engine
