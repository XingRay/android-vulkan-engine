//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "VulkanDescriptorBindingSet.h"

namespace vklite {

    class VulkanDescriptorBindingSets {
    private:
        // set -> VulkanDescriptorBindingSet
//        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> mVulkanDescriptorBindingSets;

    public:
        VulkanDescriptorBindingSets();

        ~VulkanDescriptorBindingSets();

//        [[nodiscard]]
//        const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> &getVulkanDescriptorBindingSets() const;
//
//        VulkanDescriptorBindingSets &set(uint32_t set, std::unique_ptr<VulkanDescriptorBindingSet> &&vulkanDescriptorBindingSet);
//
//        VulkanDescriptorBindingSets &updateUniformBuffer(const VulkanCommandPool& commandPool, uint32_t set, uint32_t binding, const void *data, uint32_t size);

    };

} // engine
