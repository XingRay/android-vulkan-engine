//
// Created by leixing on 2025/3/28.
//

#include "VulkanDescriptorBindingSet.h"

namespace vklite {

    VulkanDescriptorBindingSet::VulkanDescriptorBindingSet() = default;

    VulkanDescriptorBindingSet::~VulkanDescriptorBindingSet() = default;

//    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBinding>> &VulkanDescriptorBindingSet::getVulkanDescriptorBindings() const {
//        return mVulkanDescriptorBindings;
//    }
//
//    VulkanDescriptorBindingSet &VulkanDescriptorBindingSet::set(uint32_t binding, std::unique_ptr<VulkanDescriptorBinding> &&vulkanDescriptorBinding) {
//        mVulkanDescriptorBindings[binding] = std::move(vulkanDescriptorBinding);
//        return *this;
//    }
//
//    VulkanDescriptorBindingSet &VulkanDescriptorBindingSet::updateUniformBuffer(const VulkanCommandPool& commandPool, uint32_t binding, const void *data, uint32_t size) {
//        mVulkanDescriptorBindings[binding]->updateUniformBuffer(commandPool, data, size);
//        return *this;
//    }

} // engine