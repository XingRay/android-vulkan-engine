//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>

#include "engine/ImageInterface.h"

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/image/VulkanImageBuilderInterface.h"
#include "engine/vulkan_wrapper/sampler/SamplerInterface.h"
#include "engine/vulkan_wrapper/sampler/builder/SamplerBuilderInterface.h"

#include "engine/vulkan_wrapper/pipeline/resource/ImageInfo.h"

namespace engine {

    class VulkanDescriptorImmutableSamplerConfigure {
    private:

        // set ImageInfo
        std::unique_ptr<ImageInfo> mVulkanDescriptorBufferInfo;

        //create ImageInfo
        std::unique_ptr<VulkanImageBuilderInterface> mVulkanImageBuilder;
        std::unique_ptr<SamplerBuilderInterface> mVulkanSamplerBuilder;

        // image data
        std::shared_ptr<ImageInterface> mImage;

    public:
        VulkanDescriptorImmutableSamplerConfigure(
                std::unique_ptr<VulkanImageBuilderInterface> &&vulkanImageBuilder,
                std::unique_ptr<SamplerBuilderInterface> &&vulkanSamplerBuilder,
                std::unique_ptr<engine::ImageInterface> &&image);

        ~VulkanDescriptorImmutableSamplerConfigure();

        std::unique_ptr<ImageInfo> provideVulkanDescriptorImageInfo(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);
    };

} // engine
