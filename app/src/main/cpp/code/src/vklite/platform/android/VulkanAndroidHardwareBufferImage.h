//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "vklite/instance/VulkanInstance.h"
#include "vklite/device/VulkanDevice.h"
#include "vklite/command/VulkanCommandPool.h"
#include "vklite/buffer/host_visible/VulkanHostVisibleBuffer.h"
#include "vklite/image/VulkanImage.h"

#include "AndroidHardwareBuffer.h"
#include "VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "vklite/image/VulkanImageInterface.h"

namespace vklite {

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
