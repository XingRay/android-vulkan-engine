//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBindingSet.h"

namespace engine {

    class VulkanDescriptorBindingSets {
    private:
        // set -> VulkanDescriptorBindingSet
        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> mVulkanDescriptorBindingSets;

    public:
        VulkanDescriptorBindingSets();

        ~VulkanDescriptorBindingSets();

        [[nodiscard]]
        const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> &getVulkanDescriptorBindingSets() const;

        VulkanDescriptorBindingSets &set(uint32_t set, std::unique_ptr<VulkanDescriptorBindingSet> &&vulkanDescriptorBindingSet);
    };

} // engine
