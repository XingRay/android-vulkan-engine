//
// Created by leixing on 2025/1/19.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanBuffer.h"

namespace engine {

    class VulkanHardwareBuffer : public VulkanBuffer {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Image mImage;
        vk::DeviceMemory mMemory;

    public:
        VulkanHardwareBuffer(const VulkanInstance &vulkanInstance, const VulkanDevice &vulkanDevice, const AHardwareBuffer *hardwareBuffer, uint32_t width, uint32_t height, uint32_t binding,
                             VulkanBufferType type, uint32_t index);

        ~VulkanHardwareBuffer();
    };

} // engine
