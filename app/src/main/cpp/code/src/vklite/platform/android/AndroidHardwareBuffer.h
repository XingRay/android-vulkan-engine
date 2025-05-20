//
// Created by leixing on 2025/3/16.
//

#pragma once

#include <android/hardware_buffer.h>

#include "vklite/device/VulkanDevice.h"

namespace vklite {

    class AndroidHardwareBuffer {
    private:
        const VulkanDevice &mVulkanDevice;
        AHardwareBuffer *mHardwareBuffer;

    public:
        AndroidHardwareBuffer(const VulkanDevice &vulkanDevice, AHardwareBuffer *hardwareBuffer);

        ~AndroidHardwareBuffer();

        [[nodiscard]]
        vk::AndroidHardwareBufferPropertiesANDROID getAndroidHardwareBufferProperties() const;

        [[nodiscard]]
        vk::AndroidHardwareBufferFormatPropertiesANDROID getAndroidHardwareBufferFormatProperties() const;

        [[nodiscard]]
        AHardwareBuffer_Desc getAndroidHardwareBufferDescription() const;

        [[nodiscard]]
        uint32_t getDataSize() const;

        AHardwareBuffer *getHardwareBuffer() const;
    };

} // engine