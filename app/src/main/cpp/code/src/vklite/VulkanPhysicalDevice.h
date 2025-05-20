//
// Created by leixing on 2025/1/9.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include <optional>

#include "vklite/configure/physical_device/VulkanPhysicalDeviceSurfaceSupport.h"
#include "VulkanSurface.h"

namespace vklite {

    class VulkanPhysicalDevice {
    private:
        vk::PhysicalDevice mPhysicalDevice;

    public:

        explicit VulkanPhysicalDevice(const vk::PhysicalDevice &physicalDevice);

        ~VulkanPhysicalDevice();

        std::optional<VulkanPhysicalDeviceSurfaceSupport> querySurfaceSupport(const VulkanSurface &vulkanSurface, vk::QueueFlags requiredQueueFlags) const;

        bool isSupportExtensions(const std::vector<std::string> extensions) const;

        [[nodiscard]]
        const vk::PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]]
        vk::SampleCountFlagBits queryMaxUsableSampleCount() const;

        [[nodiscard]]
        std::vector<uint32_t> querySupportedSampleCounts() const;

        [[nodiscard]]
        std::string queryName() const;

        [[nodiscard]]
        vk::PhysicalDeviceType queryType() const;

        void printInfo() const;

        void printInfoWithSurface(const vk::SurfaceKHR &surface) const;

        [[nodiscard]]
        std::vector<std::string> queryExtensionNames() const;
    };

} // engine
