//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <unordered_map>

#include "engine/vulkan_wrapper/VulkanBufferDescriptorBinding.h"

namespace engine {

    class VulkanBufferDescriptorBindingSet {
    private:
        // binding -> VulkanBufferDescriptorBinding
        std::unordered_map<uint32_t, std::unique_ptr<VulkanBufferDescriptorBinding>> mVulkanBufferDescriptorBindings;

    public:
        VulkanBufferDescriptorBindingSet();

        ~VulkanBufferDescriptorBindingSet();

        [[nodiscard]]
        const std::unordered_map<uint32_t, std::unique_ptr<VulkanBufferDescriptorBinding>> &getVulkanBufferDescriptorBindings() const;

        VulkanBufferDescriptorBindingSet &set(uint32_t binding, std::unique_ptr<VulkanBufferDescriptorBinding> &&vulkanBufferDescriptorBinding);
    };

} // engine
