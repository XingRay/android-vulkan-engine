//
// Created by leixing on 2025/3/31.
//

#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <functional>

#include "engine/vulkan_wrapper/VulkanBufferView.h"
#include "engine/VulkanBufferBuilder.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    class VulkanBufferViewConfigure {
    private:
        // set BufferView
        std::unique_ptr<VulkanBufferView> mVulkanBufferView;

        // create Buffer
        std::unique_ptr<VulkanBufferBuilder> mVulkanBufferBuilder;
        vk::DeviceSize mVulkanBufferCapacity;

        // create BufferView
        uint32_t mVulkanBufferOffset;
        uint32_t mVulkanBufferRange;

        // update buffer
        std::vector<uint8_t> mVulkanBufferData;

    public:

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView, const void *data, uint32_t size);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView, std::vector<uint8_t>&& data);

        explicit VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                  uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, const void *data, uint32_t size);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                  uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, std::vector<uint8_t>&& data);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                  uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                  const void *data, uint32_t size);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                  std::vector<uint8_t>&& data);

        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity);

        template<class T>
        VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity, const T& data)
                :VulkanBufferViewConfigure(vulkanBufferBuilder, vulkanBufferCapacity, &data, sizeof(T)) {}

        ~VulkanBufferViewConfigure();

        [[nodiscard]]
        std::unique_ptr<VulkanBufferView> providerVulkanBufferView(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool);

    private:
        VulkanBufferViewConfigure& copyData(const void *data, uint32_t size);
    };

} // engine
