//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>

#include "engine/ImageInterface.h"

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/image/VulkanImageBuilderInterface.h"
#include "engine/vulkan_wrapper/image/VulkanSamplerBuilderInterface.h"

#include "engine/vulkan_wrapper/image/VulkanDescriptorImageInfo.h"

namespace engine {

    class VulkanDescriptorImageInfoConfigure {
    private:
//        // set bufferInfo
//        std::unique_ptr<VulkanDescriptorBufferInfo> mVulkanDescriptorBufferInfo;
//
//        // create Buffer
//        std::unique_ptr<VulkanBufferBuilder> mVulkanBufferBuilder;
//        vk::DeviceSize mVulkanBufferCapacity;
//
//        // create bufferInfo
//        uint32_t mVulkanBufferOffset;
//        uint32_t mVulkanBufferRange;
//
//        // update buffer
//        std::vector<uint8_t> mVulkanBufferData;

        // set ImageInfo
        std::unique_ptr<VulkanDescriptorImageInfo> mVulkanDescriptorBufferInfo;

        //create ImageInfo
        std::unique_ptr<VulkanImageBuilderInterface> mVulkanImageBuilder;
        std::unique_ptr<VulkanSamplerBuilderInterface> mVulkanSamplerBuilder;


        // create ImageView
//        std::unique_ptr<VulkanBufferBuilder> mVulkanBufferBuilder;
//        vk::DeviceSize mVulkanBufferCapacity;

        // create BufferView
//        uint32_t mVulkanBufferOffset;
//        uint32_t mVulkanBufferRange;

        // update buffer
//        std::vector<uint8_t> mVulkanBufferData;

        std::shared_ptr<ImageInterface> mImage;

    public:
        VulkanDescriptorImageInfoConfigure(
                std::unique_ptr<VulkanImageBuilderInterface> &&vulkanImageBuilder,
                std::unique_ptr<VulkanSamplerBuilderInterface> &&vulkanSamplerBuilder,
                std::unique_ptr<engine::ImageInterface> &&image);

        ~VulkanDescriptorImageInfoConfigure();

        std::unique_ptr<VulkanDescriptorImageInfo> provideVulkanDescriptorImageInfo(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);
    };

} // engine
