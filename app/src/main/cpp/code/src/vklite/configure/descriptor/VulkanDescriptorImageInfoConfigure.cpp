//
// Created by leixing on 2025/4/8.
//

#include "VulkanDescriptorImageInfoConfigure.h"

namespace vklite {

    VulkanDescriptorImageInfoConfigure::VulkanDescriptorImageInfoConfigure(
            std::unique_ptr<VulkanImageBuilderInterface> &&vulkanImageBuilder,
            std::unique_ptr<SamplerBuilderInterface> &&vulkanSamplerBuilder,
            std::unique_ptr<vklite::ImageInterface> &&image)
            : mVulkanImageBuilder(std::move(vulkanImageBuilder)),
              mVulkanSamplerBuilder(std::move(vulkanSamplerBuilder)),
              mImage(std::move(image)) {}

    VulkanDescriptorImageInfoConfigure::~VulkanDescriptorImageInfoConfigure() = default;

    std::unique_ptr<ImageInfo> VulkanDescriptorImageInfoConfigure::provideVulkanDescriptorImageInfo(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
        if(mVulkanDescriptorBufferInfo!= nullptr){
            return std::move(mVulkanDescriptorBufferInfo);
        }

        std::unique_ptr<VulkanImageInterface> vulkanImage = mVulkanImageBuilder->build(vulkanDevice, mImage->getWidth(), mImage->getHeight(), mImage->getFormat());
        vulkanImage->transitionImageLayout(commandPool);
        vulkanImage->update(commandPool, mImage->getPixels(), mImage->getPixelBytes());
        std::unique_ptr<SamplerInterface> sampler = mVulkanSamplerBuilder->build(vulkanDevice);
        return std::make_unique<ImageInfo>(std::move(vulkanImage), std::move(sampler));
    }

} // engine