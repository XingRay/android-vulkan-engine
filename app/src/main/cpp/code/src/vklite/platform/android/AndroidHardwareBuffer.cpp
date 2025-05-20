//
// Created by leixing on 2025/3/16.
//

#include "AndroidHardwareBuffer.h"
#include "vklite/VkCheck.h"

namespace vklite {
    AndroidHardwareBuffer::AndroidHardwareBuffer(const VulkanDevice &vulkanDevice, AHardwareBuffer *hardwareBuffer)
            : mVulkanDevice(vulkanDevice), mHardwareBuffer(hardwareBuffer) {

    }

    AndroidHardwareBuffer::~AndroidHardwareBuffer() = default;

    vk::AndroidHardwareBufferPropertiesANDROID AndroidHardwareBuffer::getAndroidHardwareBufferProperties() const {
        vk::Device device = mVulkanDevice.getDevice();
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, mHardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
        return propertiesInfo;
    }

    vk::AndroidHardwareBufferFormatPropertiesANDROID AndroidHardwareBuffer::getAndroidHardwareBufferFormatProperties() const {
        vk::Device device = mVulkanDevice.getDevice();
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, mHardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
        return formatInfo;
    }

    AHardwareBuffer_Desc AndroidHardwareBuffer::getAndroidHardwareBufferDescription() const {
        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(mHardwareBuffer, &hardwareBufferDescription);
        return hardwareBufferDescription;
    }

    uint32_t AndroidHardwareBuffer::getDataSize() const {
        AHardwareBuffer_Desc hardwareBufferDescription = getAndroidHardwareBufferDescription();
        return hardwareBufferDescription.width * hardwareBufferDescription.height * hardwareBufferDescription.layers;
    }

    AHardwareBuffer *AndroidHardwareBuffer::getHardwareBuffer() const {
        return mHardwareBuffer;
    }

} // engine