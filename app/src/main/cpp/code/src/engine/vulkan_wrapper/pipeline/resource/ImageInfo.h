//
// Created by leixing on 2025/4/9.
//

#pragma once

#include <memory>
#include "engine/vulkan_wrapper/image/VulkanImageInterface.h"
#include "engine/vulkan_wrapper/sampler/SamplerInterface.h"

namespace engine {

    class ImageInfo {
    private:
        std::unique_ptr<VulkanImageInterface> mVulkanImage;
        std::unique_ptr<SamplerInterface> mVulkanSampler;

    public:
        ImageInfo(std::unique_ptr<VulkanImageInterface> &&vulkanImage,
                  std::unique_ptr<SamplerInterface> &&vulkanSampler);

        ~ImageInfo();

        const std::unique_ptr<VulkanImageInterface> &getVulkanImage() const;

        const std::unique_ptr<SamplerInterface> &getVulkanSampler() const;

        [[nodiscard]]
        vk::DescriptorImageInfo createDescriptorImageInfo() const;
    };

} // engine
