//
// Created by leixing on 2025/3/31.
//

#include "VulkanBufferViewConfigure.h"

namespace engine {

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView, const void *data, uint32_t size)
            : mVulkanBufferView(std::move(bufferView)),
              mVulkanBufferCapacity(0), mVulkanBufferOffset(0), mVulkanBufferRange(0) {

    }

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView, std::vector<uint8_t> &&data)
            : mVulkanBufferView(std::move(bufferView)), mVulkanBufferData(std::move(data)),
              mVulkanBufferCapacity(0), mVulkanBufferOffset(0), mVulkanBufferRange(0) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView)
            : mVulkanBufferView(std::move(bufferView)),
              mVulkanBufferCapacity(0), mVulkanBufferOffset(0), mVulkanBufferRange(0) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, const void *data, uint32_t size)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(vulkanBufferOffset), mVulkanBufferRange(vulkanBufferRange) {
        copyData(data, size);
    }

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, std::vector<uint8_t> &&data)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(vulkanBufferOffset), mVulkanBufferRange(vulkanBufferRange),
              mVulkanBufferData(std::move(data)) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(vulkanBufferOffset), mVulkanBufferRange(vulkanBufferRange) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         const void *data, uint32_t size)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(0), mVulkanBufferRange(size) {
        copyData(data, size);
    }

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         std::vector<uint8_t> &&data)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)),
              mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(0), mVulkanBufferRange(data.size()),
              mVulkanBufferData(std::move(data)) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferBuilder> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity)
            : mVulkanBufferBuilder(std::move(vulkanBufferBuilder)), mVulkanBufferCapacity(vulkanBufferCapacity), mVulkanBufferOffset(0), mVulkanBufferRange(vulkanBufferCapacity) {}

    VulkanBufferViewConfigure::~VulkanBufferViewConfigure() = default;

    std::unique_ptr<VulkanBufferView> VulkanBufferViewConfigure::providerVulkanBufferView(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) {
        if (mVulkanBufferView != nullptr) {
            if (!mVulkanBufferData.empty()) {
                mVulkanBufferView->update(commandPool, mVulkanBufferData.data(), mVulkanBufferData.size());
            }
            return std::move(mVulkanBufferView);
        }

        if (mVulkanBufferBuilder == nullptr) {
            return nullptr;
        }

        std::unique_ptr<VulkanBufferInterface> vulkanBuffer = mVulkanBufferBuilder->build(vulkanDevice, mVulkanBufferCapacity);
        if (!mVulkanBufferData.empty()) {
            vulkanBuffer->update(commandPool, mVulkanBufferData.data(), mVulkanBufferData.size());
        }
        return std::make_unique<VulkanBufferView>(std::move(vulkanBuffer), mVulkanBufferOffset, mVulkanBufferRange);
    }

    VulkanBufferViewConfigure &VulkanBufferViewConfigure::copyData(const void *data, uint32_t size) {
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