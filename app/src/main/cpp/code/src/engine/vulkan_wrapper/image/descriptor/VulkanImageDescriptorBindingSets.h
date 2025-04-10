//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "VulkanImageDescriptorBindingSet.h"

namespace engine {

    class VulkanImageDescriptorBindingSets {
    private:
        // set -> VulkanBufferDescriptorBindingSet
        std::unordered_map<uint32_t, std::unique_ptr<VulkanImageDescriptorBindingSet>> mVulkanImageDescriptorBindingSets;

    public:
        VulkanImageDescriptorBindingSets();

        ~VulkanImageDescriptorBindingSets();

        [[nodiscard]]
        const std::unordered_map<uint32_t, std::unique_ptr<VulkanImageDescriptorBindingSet>> &getVulkanImageDescriptorBindingSets() const;

        VulkanImageDescriptorBindingSets &set(uint32_t set, std::unique_ptr<VulkanImageDescriptorBindingSet> &&vulkanImageDescriptorBindingSet);
    };

} // engine
