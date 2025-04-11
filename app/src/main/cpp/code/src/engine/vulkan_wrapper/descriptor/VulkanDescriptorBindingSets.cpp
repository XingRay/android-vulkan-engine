//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBindingSets.h"

namespace engine {

    VulkanDescriptorBindingSets::VulkanDescriptorBindingSets() = default;

    VulkanDescriptorBindingSets::~VulkanDescriptorBindingSets() = default;

    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> &VulkanDescriptorBindingSets::getVulkanDescriptorBindingSets() const {
        return mVulkanDescriptorBindingSets;
    }

    VulkanDescriptorBindingSets &VulkanDescriptorBindingSets::set(uint32_t set, std::unique_ptr<VulkanDescriptorBindingSet> &&vulkanDescriptorBindingSet) {
        mVulkanDescriptorBindingSets[set] = std::move(vulkanDescriptorBindingSet);
        return *this;
    }

} // engine