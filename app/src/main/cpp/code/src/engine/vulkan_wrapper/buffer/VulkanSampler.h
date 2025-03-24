//
// Created by leixing on 2025/1/7.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

#include "engine/vulkan_wrapper/buffer/VulkanImage.h"
#include "engine/vulkan_wrapper/buffer/VulkanStagingBuffer.h"

namespace engine {

    class VulkanSampler {
    private:
        const VulkanDevice &mVulkanDevice;

//        uint32_t mImageSize;
        VulkanImage mVulkanImage;
        vk::Sampler mSampler;

        VulkanStagingBuffer mVulkanStagingBuffer;

    public:

        VulkanSampler(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, uint32_t channels);

        ~VulkanSampler();

        [[nodiscard]]
        const vk::Image &getImage() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        const vk::ImageView &getImageView() const;

        [[nodiscard]]
        const vk::Sampler &getSampler() const;

        template<class T>
        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<T> &data) {
            recordCommandUpdate(commandBuffer, data.data(), data.size() * sizeof(T));
        }

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size);

        std::vector<vk::DescriptorImageInfo> createDescriptorImageInfos();

        void generateMipmaps(const VulkanCommandPool &vulkanCommandPool);

        void recordCommandGenerateMipmaps(const vk::CommandBuffer &commandBuffer);
    };

} // engine
