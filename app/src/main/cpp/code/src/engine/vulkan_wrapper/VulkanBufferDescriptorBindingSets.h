//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "engine/vulkan_wrapper/VulkanBufferDescriptorBindingSet.h"

namespace engine {

    class VulkanBufferDescriptorBindingSets {
    private:
        // set -> VulkanBufferDescriptorBindingSet
        std::unordered_map<uint32_t, std::unique_ptr<VulkanBufferDescriptorBindingSet>> mVulkanBufferDescriptorBindingSets;

    public:
        VulkanBufferDescriptorBindingSets();

        ~VulkanBufferDescriptorBindingSets();

        VulkanBufferDescriptorBindingSets& set(uint32_t set, std::unique_ptr<VulkanBufferDescriptorBindingSet>&& vulkanBufferDescriptorBindingSet);
    };

} // engine
