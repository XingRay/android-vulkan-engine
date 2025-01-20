//
// Created by leixing on 2025/1/19.
//

#include <vulkan/vulkan_hpp_macros.hpp>
#include "VulkanHardwareBuffer.h"
#include "engine/VkCheckCpp.h"
#include "engine/VkCheck.h"

#include "engine/VulkanUtil.h"

namespace engine {

    VulkanHardwareBuffer::VulkanHardwareBuffer(const VulkanInstance &vulkanInstance, const VulkanDevice &vulkanDevice, const AHardwareBuffer *hardwareBuffer, uint32_t width, uint32_t height,
                                               uint32_t binding, VulkanBufferType type,
                                               uint32_t index)
            : mVulkanDevice(vulkanDevice), VulkanBuffer(binding, type, index) {

        const vk::Device &device = vulkanDevice.getDevice();

        // 获取 HardwareBuffer 属性
        VkAndroidHardwareBufferPropertiesANDROID hardwareBufferProperties;

        const vk::Instance &instance = vulkanInstance.getInstance();
        static auto vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(
                instance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, &hardwareBufferProperties));

//        CALL_VK_CPP(device.getAndroidHardwareBufferPropertiesANDROID(hardwareBuffer, &hardwareBufferProperties));

        // 创建 Vulkan 图像
        vk::ImageCreateInfo imageCreateInfo(
                {},                                      // 标志
                vk::ImageType::e2D,                     // 图像类型
                vk::Format::eG8B8R83Plane420Unorm,      // 格式
                vk::Extent3D(width, height, 1),         // 尺寸
                1,                                      // Mip 级别
                1,                                      // 数组层数
                vk::SampleCountFlagBits::e1,            // 采样数
                vk::ImageTiling::eOptimal,              // 图像布局
                vk::ImageUsageFlagBits::eSampled,       // 用途
                vk::SharingMode::eExclusive,            // 共享模式
                0, nullptr,                             // 队列族索引
                vk::ImageLayout::eUndefined             // 初始布局
        );

        mImage = device.createImage(imageCreateInfo);

        // 获取图像的内存需求
        vk::PhysicalDeviceMemoryProperties memProperties = vulkanDevice.getPhysicalDevice().getMemoryProperties();
        vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(mImage);
        uint32_t memoryType = VulkanUtil::findMemoryType(memProperties, memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        // 分配内存并绑定到图像
        vk::MemoryAllocateInfo memoryAllocateInfo(hardwareBufferProperties.allocationSize, memoryType);

        mMemory = device.allocateMemory(memoryAllocateInfo);
        device.bindImageMemory(mImage, mMemory, 0);

    }

    VulkanHardwareBuffer::~VulkanHardwareBuffer() {
        vk::Device device = mVulkanDevice.getDevice();
        device.destroyImage(mImage);
        device.freeMemory(mMemory);
    }
} // engine