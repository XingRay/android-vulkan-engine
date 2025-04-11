//
// Created by leixing on 2025/4/9.
//

#pragma once

#include <memory>
#include "engine/vulkan_wrapper/image/VulkanImageInterface.h"
#include "engine/vulkan_wrapper/image/VulkanSamplerInterface.h"

namespace engine {

    class VulkanDescriptorImageInfo {
    private:
        std::unique_ptr<VulkanImageInterface> mVulkanImage;
        std::unique_ptr<VulkanSamplerInterface> mVulkanSampler;

    public:
        VulkanDescriptorImageInfo(std::unique_ptr<VulkanImageInterface> &&vulkanImage,
                                  std::unique_ptr<VulkanSamplerInterface> &&vulkanSampler);

        ~VulkanDescriptorImageInfo();

        [[nodiscard]]
        vk::DescriptorImageInfo createDescriptorImageInfo() const;
    };

} // engine
