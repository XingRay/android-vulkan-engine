//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleBuffer.h"
#include "engine/vulkan_wrapper/buffer/VulkanImage.h"

#include "engine/vulkan_wrapper/platform/android/AndroidHardwareBuffer.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidSamplerYcbcrConversion.h"

namespace engine {

    class VulkanAndroidHardwareBufferImage {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
        vk::ImageView mImageView;

    public:
        VulkanAndroidHardwareBufferImage(const VulkanDevice &vulkanDevice,
                                         const AndroidHardwareBuffer& androidHardwareBuffer,
                                         const VulkanAndroidSamplerYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~VulkanAndroidHardwareBufferImage();

        [[nodiscard]]
        const vk::Image &getImage() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        const vk::ImageView &getImageView() const;
    };

} // engine
