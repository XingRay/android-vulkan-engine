//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanBufferView.h"

namespace engine {

    class VulkanBufferDescriptorBinding {
    private:
        // buffer view
        std::unique_ptr<VulkanBufferView> mVulkanBufferView;

        //descriptor view
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

    public:
        VulkanBufferDescriptorBinding();

        ~VulkanBufferDescriptorBinding();

        [[nodiscard]]
        const std::unique_ptr<VulkanBufferView> &getVulkanBufferView() const;

        VulkanBufferDescriptorBinding &setBufferView(std::unique_ptr<VulkanBufferView> &&vulkanBufferView);

        [[nodiscard]]
        vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet, uint32_t binding) const;
    };

} // engine
