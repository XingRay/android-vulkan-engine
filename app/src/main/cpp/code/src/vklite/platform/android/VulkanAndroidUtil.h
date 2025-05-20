//
// Created by leixing on 2025/3/9.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "android/hardware_buffer.h"

namespace vklite {

    class VulkanAndroidUtil {
    public:

        [[nodiscard]]
        static vk::AndroidHardwareBufferFormatPropertiesANDROID getAndroidHardwareBufferFormat(vk::Device device, AHardwareBuffer *hardwareBuffer);
    };

} // engine

