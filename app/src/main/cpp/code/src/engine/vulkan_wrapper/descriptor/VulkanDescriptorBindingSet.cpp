//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBindingSet.h"

namespace engine {

    VulkanDescriptorBindingSet::VulkanDescriptorBindingSet() = default;

    VulkanDescriptorBindingSet::~VulkanDescriptorBindingSet() = default;

    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBinding>> &VulkanDescriptorBindingSet::getVulkanDescriptorBindings() const {
        return mVulkanDescriptorBindings;
    }

    VulkanDescriptorBindingSet &VulkanDescriptorBindingSet::set(uint32_t binding, std::unique_ptr<VulkanDescriptorBinding> &&vulkanDescriptorBinding) {
        mVulkanDescriptorBindings[binding] = std::move(vulkanDescriptorBinding);
        return *this;
    }

} // engine