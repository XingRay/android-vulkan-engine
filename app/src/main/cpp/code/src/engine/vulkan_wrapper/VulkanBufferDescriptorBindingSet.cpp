//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/VulkanBufferDescriptorBindingSet.h"

namespace engine {

    VulkanBufferDescriptorBindingSet::VulkanBufferDescriptorBindingSet() = default;

    VulkanBufferDescriptorBindingSet::~VulkanBufferDescriptorBindingSet() = default;

    VulkanBufferDescriptorBindingSet &VulkanBufferDescriptorBindingSet::set(uint32_t binding, std::unique_ptr<VulkanBufferDescriptorBinding> &&vulkanBufferDescriptorBinding) {
        mVulkanBufferDescriptorBindings[binding] = std::move(vulkanBufferDescriptorBinding);
        return *this;
    }

} // engine