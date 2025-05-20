//
// Created by leixing on 2025/3/28.
//

#include "VulkanDescriptorBindingSets.h"

namespace vklite {

    VulkanDescriptorBindingSets::VulkanDescriptorBindingSets() = default;

    VulkanDescriptorBindingSets::~VulkanDescriptorBindingSets() = default;

//    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> &VulkanDescriptorBindingSets::getVulkanDescriptorBindingSets() const {
//        return mVulkanDescriptorBindingSets;
//    }
//
//    VulkanDescriptorBindingSets &VulkanDescriptorBindingSets::set(uint32_t set, std::unique_ptr<VulkanDescriptorBindingSet> &&vulkanDescriptorBindingSet) {
//        mVulkanDescriptorBindingSets[set] = std::move(vulkanDescriptorBindingSet);
//        return *this;
//    }
//
//    VulkanDescriptorBindingSets &VulkanDescriptorBindingSets::updateUniformBuffer(const VulkanCommandPool& commandPool, uint32_t set, uint32_t binding, const void *data, uint32_t size) {
//        mVulkanDescriptorBindingSets[set]->updateUniformBuffer(commandPool, binding, data, size);
//        return *this;
//    }

} // engine