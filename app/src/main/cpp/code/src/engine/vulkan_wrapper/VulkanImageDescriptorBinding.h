//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanImageView.h"

namespace engine {

    class VulkanImageDescriptorBinding {
    private:
        // buffer view
        std::unique_ptr<VulkanImageView> mVulkanImageView;

        //descriptor view
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

    public:
        VulkanImageDescriptorBinding(std::unique_ptr<VulkanImageView> &&vulkanImageView,
                                     vk::DescriptorType descriptorType,
                                     uint32_t descriptorOffset,
                                     uint32_t descriptorRange);

        ~VulkanImageDescriptorBinding();

        [[nodiscard]]
        const std::unique_ptr<VulkanImageView> &getVulkanImageView() const;

        VulkanImageDescriptorBinding &setImageView(std::unique_ptr<VulkanImageView> &&vulkanImageView);

        [[nodiscard]]
        vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet, uint32_t binding) const;
    };

} // engine
