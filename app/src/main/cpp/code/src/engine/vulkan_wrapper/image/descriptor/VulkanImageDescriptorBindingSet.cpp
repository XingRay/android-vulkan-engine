//
// Created by leixing on 2025/3/28.
//

#include "VulkanImageDescriptorBindingSet.h"

namespace engine {

    VulkanImageDescriptorBindingSet::VulkanImageDescriptorBindingSet() = default;

    VulkanImageDescriptorBindingSet::~VulkanImageDescriptorBindingSet() = default;

    const std::unordered_map<uint32_t, std::unique_ptr<VulkanImageDescriptorBinding>> &VulkanImageDescriptorBindingSet::getVulkanImageDescriptorBindings() const {
        return mVulkanImageDescriptorBindings;
    }

    VulkanImageDescriptorBindingSet &VulkanImageDescriptorBindingSet::set(uint32_t binding, std::unique_ptr<VulkanImageDescriptorBinding> &&vulkanImageDescriptorBinding) {
        mVulkanImageDescriptorBindings[binding] = std::move(vulkanImageDescriptorBinding);
        return *this;
    }

} // engine