//
// Created by leixing on 2025/4/9.
//

#pragma once

#include <memory>
#include "engine/vulkan_wrapper/buffer/VulkanImage.h"
#include "engine/vulkan_wrapper/buffer/VulkanSampler.h"

namespace engine {

    class VulkanDescriptorImageInfo {
    private:
        std::shared_ptr<VulkanImage> mVulkanImage;
        std::shared_ptr<VulkanSampler> mVulkanSampler;

    public:
        VulkanDescriptorImageInfo();

        ~VulkanDescriptorImageInfo();

        vk::DescriptorImageInfo createDescriptorImageInfo() const;
    };

} // engine
