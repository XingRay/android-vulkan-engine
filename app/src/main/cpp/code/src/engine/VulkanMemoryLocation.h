//
// Created by leixing on 2025/1/22.
//

#pragma once

#include <cstdint>

#include "vulkan/vulkan.hpp"

namespace engine {

    enum class VulkanMemoryLocation {
        device,
        host,
        external,


    };

    class VulkanMemoryLocationUtil {
        static int calcMemoryIndex(VulkanMemoryLocation location, const vk::PhysicalDevice& physicalDevice, uint32_t memoryTypeBits);
    };

} // engine
