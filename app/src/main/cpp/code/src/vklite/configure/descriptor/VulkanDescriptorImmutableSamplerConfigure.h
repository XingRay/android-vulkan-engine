//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>

#include "vklite/ImageInterface.h"

#include "vklite/device/VulkanDevice.h"
#include "vklite/command/VulkanCommandPool.h"
#include "vklite/image/VulkanImageBuilderInterface.h"
#include "vklite/sampler/SamplerInterface.h"
#include "vklite/sampler/builder/SamplerBuilderInterface.h"

#include "vklite/pipeline_resource/ImageInfo.h"

namespace vklite {

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
                std::unique_ptr<vklite::ImageInterface> &&image);

        ~VulkanDescriptorImmutableSamplerConfigure();

        std::unique_ptr<ImageInfo> provideVulkanDescriptorImageInfo(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);
    };

} // engine
