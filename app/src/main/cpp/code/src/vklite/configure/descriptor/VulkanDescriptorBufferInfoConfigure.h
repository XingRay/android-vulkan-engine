//
// Created by leixing on 2025/3/31.
//

#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <functional>

#include "vklite/pipeline_resource/BufferInfo.h"
#include "vklite/buffer/VulkanBufferBuilderInterface.h"
#include "vklite/device/VulkanDevice.h"
#include "vklite/command/VulkanCommandPool.h"

namespace vklite {

    class VulkanDescriptorBufferInfoConfigure {
    private:
        // set bufferInfo
        std::unique_ptr<BufferInfo> mVulkanDescriptorBufferInfo;

        // create Buffer
        std::unique_ptr<VulkanBufferBuilderInterface> mVulkanBufferBuilder;
        vk::DeviceSize mVulkanBufferCapacity;

        // create bufferInfo
        uint32_t mVulkanBufferOffset;
        uint32_t mVulkanBufferRange;

        // update buffer
        std::vector<uint8_t> mVulkanBufferData;

    public:

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, const void *data, uint32_t size);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, std::vector<uint8_t> &&data);

        explicit VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                            uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, const void *data, uint32_t size);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                            uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, std::vector<uint8_t> &&data);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                            uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                            const void *data, uint32_t size);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                            std::vector<uint8_t> &&data);

        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity);

        template<class T>
        VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity, const T &data)
                :VulkanDescriptorBufferInfoConfigure(vulkanBufferBuilder, vulkanBufferCapacity, &data, sizeof(T)) {}

        ~VulkanDescriptorBufferInfoConfigure();

        [[nodiscard]]
        std::unique_ptr<BufferInfo> provideVulkanDescriptorBufferInfo(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);

    private:
        VulkanDescriptorBufferInfoConfigure &copyData(const void *data, uint32_t size);
    };

} // engine
