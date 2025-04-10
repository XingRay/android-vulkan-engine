//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <unordered_map>

#include "engine/vulkan_wrapper/VulkanImageDescriptorBinding.h"

namespace engine {

    class VulkanImageDescriptorBindingSet {
    private:
        // binding -> VulkanBufferDescriptorBinding
        std::unordered_map<uint32_t, std::unique_ptr<VulkanImageDescriptorBinding>> mVulkanImageDescriptorBindings;

    public:
        VulkanImageDescriptorBindingSet();

        ~VulkanImageDescriptorBindingSet();

        [[nodiscard]]
        const std::unordered_map<uint32_t, std::unique_ptr<VulkanImageDescriptorBinding>> &getVulkanImageDescriptorBindings() const;

        VulkanImageDescriptorBindingSet &set(uint32_t binding, std::unique_ptr<VulkanImageDescriptorBinding> &&vulkanBufferDescriptorBinding);
    };

} // engine
