//
// Created by leixing on 2025/3/28.
//

#include "VulkanImageDescriptorBindingSets.h"

namespace engine {

    VulkanImageDescriptorBindingSets::VulkanImageDescriptorBindingSets() = default;

    VulkanImageDescriptorBindingSets::~VulkanImageDescriptorBindingSets() = default;

    const std::unordered_map<uint32_t, std::unique_ptr<VulkanImageDescriptorBindingSet>> &VulkanImageDescriptorBindingSets::getVulkanImageDescriptorBindingSets() const {
        return mVulkanImageDescriptorBindingSets;
    }

    VulkanImageDescriptorBindingSets &VulkanImageDescriptorBindingSets::set(uint32_t set, std::unique_ptr<VulkanImageDescriptorBindingSet> &&vulkanImageDescriptorBindingSet) {
        mVulkanImageDescriptorBindingSets[set] = std::move(vulkanImageDescriptorBindingSet);
        return *this;
    }

} // engine