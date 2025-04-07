//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/VulkanBufferDescriptorBindingSets.h"

namespace engine {

    VulkanBufferDescriptorBindingSets::VulkanBufferDescriptorBindingSets() = default;

    VulkanBufferDescriptorBindingSets::~VulkanBufferDescriptorBindingSets() = default;

    const std::unordered_map<uint32_t, std::unique_ptr<VulkanBufferDescriptorBindingSet>> &VulkanBufferDescriptorBindingSets::getVulkanBufferDescriptorBindingSets() const {
        return mVulkanBufferDescriptorBindingSets;
    }

    VulkanBufferDescriptorBindingSets &VulkanBufferDescriptorBindingSets::set(uint32_t set, std::unique_ptr<VulkanBufferDescriptorBindingSet> &&vulkanBufferDescriptorBindingSet) {
        mVulkanBufferDescriptorBindingSets[set] = std::move(vulkanBufferDescriptorBindingSet);
        return *this;
    }

} // engine