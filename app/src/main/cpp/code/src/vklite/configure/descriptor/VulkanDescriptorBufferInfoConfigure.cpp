//
// Created by leixing on 2025/3/31.
//

#include "vklite/configure/descriptor/VulkanDescriptorBufferInfoConfigure.h"

namespace vklite {

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, const void *data, uint32_t size)
            : mVulkanDescriptorBufferInfo(std::move(bufferInfo)),
              mVulkanBufferCapacity(0), mVulkanBufferOffset(0), mVulkanBufferRange(0) {

    }

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, std::vector<uint8_t> &&data)
            : mVulkanDescriptorBufferInfo(std::move(bufferInfo)), mVulkanBufferData(std::move(data)),
              mVulkanBufferCapacity(0), mVulkanBufferOffset(0), mVulkanBufferRange(0) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo)
            : mVulkanDescriptorBufferInfo(std::move(bufferInfo)),
              mVulkanBufferCapacity(0), mVulkanBufferOffset(0), mVulkanBufferRange(0) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, const void *data, uint32_t size)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(vulkanBufferOffset), mVulkanBufferRange(vulkanBufferRange) {
        copyData(data, size);
    }

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, std::vector<uint8_t> &&data)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(vulkanBufferOffset), mVulkanBufferRange(vulkanBufferRange),
              mVulkanBufferData(std::move(data)) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(vulkanBufferOffset), mVulkanBufferRange(vulkanBufferRange) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         const void *data, uint32_t size)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(0), mVulkanBufferRange(size) {
        copyData(data, size);
    }

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         std::vector<uint8_t> &&data)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(0), mVulkanBufferRange(data.size()),
              mVulkanBufferData(std::move(data)) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)), mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(0), mVulkanBufferRange(vulkanBufferCapacity) {}

    VulkanDescriptorBufferInfoConfigure::~VulkanDescriptorBufferInfoConfigure() = default;

    std::unique_ptr<BufferInfo> VulkanDescriptorBufferInfoConfigure::provideVulkanDescriptorBufferInfo(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
        if (mVulkanDescriptorBufferInfo != nullptr) {
            if (!mVulkanBufferData.empty()) {
                mVulkanDescriptorBufferInfo->update(commandPool, mVulkanBufferData.data(), mVulkanBufferData.size());
            }
            return std::move(mVulkanDescriptorBufferInfo);
        }

        if (mVulkanBufferBuilder == nullptr) {
            return nullptr;
        }

        std::unique_ptr<VulkanBufferInterface> vulkanBuffer = mVulkanBufferBuilder->build(vulkanDevice, mVulkanBufferCapacity);
        if (!mVulkanBufferData.empty()) {
            vulkanBuffer->update(commandPool, mVulkanBufferData.data(), mVulkanBufferData.size());
        }
        return std::make_unique<BufferInfo>(std::move(vulkanBuffer), mVulkanBufferOffset, mVulkanBufferRange);
    }

    VulkanDescriptorBufferInfoConfigure &VulkanDescriptorBufferInfoConfigure::copyData(const void *data, uint32_t size) {
        if (data == nullptr) {
            mVulkanBufferData.clear();
            return *this;
        }

        if (size == 0) {
            mVulkanBufferData.clear();
            return *this;
        }

        mVulkanBufferData.resize(size);
        std::memcpy(mVulkanBufferData.data(), data, size);

        // or:
        // const uint8_t* byteData = static_cast<const uint8_t*>(data);
        // std::copy(byteData, byteData + size, mVulkanBufferData.begin());

        return *this;
    }

} // engine