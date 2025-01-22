//
// Created by leixing on 2025/1/22.
//

#include "VulkanMemoryLocation.h"


namespace engine {
    int VulkanMemoryLocationUtil::calcMemoryIndex(VulkanMemoryLocation location, const vk::PhysicalDevice &physicalDevice, uint32_t memoryTypeBits) {
        int memoryIndex{0};
        vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlags{0};
        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

        switch (location) {
            case VulkanMemoryLocation::device:
                memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;
                break;
            case VulkanMemoryLocation::host:
                memoryPropertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
                break;
            default:
                break;
        }

        if (location == VulkanMemoryLocation::external) {
            for (auto &type: memoryProperties.memoryTypes) {
                if ((memoryTypeBits & (1 << memoryIndex)) && ((type.propertyFlags & memoryPropertyFlags) == vk::MemoryPropertyFlags{0})) {
                    break;
                }
                memoryIndex++;
            }
        } else {
            for (auto &type: memoryProperties.memoryTypes) {
                if ((memoryTypeBits & (1 << memoryIndex)) && (type.propertyFlags & memoryPropertyFlags)) {
                    break;
                }
                memoryIndex++;
            }
        }

        return memoryIndex;
    }
} // engine