//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/VulkanBufferDescriptorBindingSet.h"

namespace engine {

    VulkanBufferDescriptorBindingSet::VulkanBufferDescriptorBindingSet() = default;

    VulkanBufferDescriptorBindingSet::~VulkanBufferDescriptorBindingSet() = default;

    const std::unordered_map<uint32_t, std::unique_ptr<VulkanBufferDescriptorBinding>> &VulkanBufferDescriptorBindingSet::getVulkanBufferDescriptorBindings() const {
        return mVulkanBufferDescriptorBindings;
    }

    VulkanBufferDescriptorBindingSet &VulkanBufferDescriptorBindingSet::set(uint32_t binding, std::unique_ptr<VulkanBufferDescriptorBinding> &&vulkanBufferDescriptorBinding) {
        mVulkanBufferDescriptorBindings[binding] = std::move(vulkanBufferDescriptorBinding);
        return *this;
    }

} // engine