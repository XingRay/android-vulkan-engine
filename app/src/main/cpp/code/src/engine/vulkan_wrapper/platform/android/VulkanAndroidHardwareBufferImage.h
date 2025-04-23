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
#include "engine/vulkan_wrapper/image/VulkanImage.h"

#include "engine/vulkan_wrapper/platform/android/AndroidHardwareBuffer.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "engine/vulkan_wrapper/image/VulkanImageInterface.h"

namespace engine {

    class VulkanAndroidHardwareBufferImage : public VulkanImageInterface {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
        vk::ImageView mImageView;

    public:
        VulkanAndroidHardwareBufferImage(const VulkanDevice &vulkanDevice,
                                         const AndroidHardwareBuffer &androidHardwareBuffer,
                                         const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~VulkanAndroidHardwareBufferImage() override;

        [[nodiscard]]
        const vk::Image &getImage() const override;

        [[nodiscard]]
        const vk::ImageView &getImageView() const override;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        uint32_t getMipLevels() const override;

        void transitionImageLayout(const VulkanCommandPool &commandPool) override;

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) override;

    };

} // engine
