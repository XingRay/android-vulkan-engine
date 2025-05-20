//
// Created by leixing on 2025/3/9.
//

#include "VulkanAndroidUtil.h"
#include "vklite/VkCheck.h"

namespace vklite {

    vk::AndroidHardwareBufferFormatPropertiesANDROID VulkanAndroidUtil::getAndroidHardwareBufferFormat(vk::Device device, AHardwareBuffer* hardwareBuffer){
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
        return formatInfo;
    }
} // engine