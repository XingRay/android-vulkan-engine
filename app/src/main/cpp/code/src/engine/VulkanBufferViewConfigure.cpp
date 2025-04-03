//
// Created by leixing on 2025/3/31.
//

#include "VulkanBufferViewConfigure.h"

namespace engine {

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(std::unique_ptr<VulkanBufferView> &&bufferView)
            : mVulkanBufferView(std::move(bufferView)), mVulkanBufferViewCreateInfo(nullptr) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(uint32_t capacity, const void *data, uint32_t size)
            : mVulkanBufferView(nullptr), mVulkanBufferViewCreateInfo(std::make_unique<VulkanBufferViewCreateInfo>(capacity, data, size)) {}

    VulkanBufferViewConfigure::VulkanBufferViewConfigure(uint32_t capacity)
            : VulkanBufferViewConfigure(capacity, nullptr, 0) {}

    VulkanBufferViewConfigure::~VulkanBufferViewConfigure() = default;

    std::unique_ptr<VulkanBufferView>
    VulkanBufferViewConfigure::getOrCreateVulkanBufferView(const std::function<std::unique_ptr<VulkanBufferView>(const VulkanBufferViewCreateInfo &)> &vulkanBufferViewBuilder) {
        if (mVulkanBufferView != nullptr) {
            return std::move(mVulkanBufferView);
        }

        return vulkanBufferViewBuilder(*mVulkanBufferViewCreateInfo);
    }

} // engine