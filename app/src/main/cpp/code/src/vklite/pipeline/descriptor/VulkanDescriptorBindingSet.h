//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <unordered_map>

#include "Descriptor.h"

namespace vklite {

    class VulkanDescriptorBindingSet {
    private:
        // binding -> VulkanDescriptorBinding
//        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBinding>> mVulkanDescriptorBindings;

    public:
        VulkanDescriptorBindingSet();

        ~VulkanDescriptorBindingSet();

//        [[nodiscard]]
//        const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBinding>> &getVulkanDescriptorBindings() const;
//
//        VulkanDescriptorBindingSet &set(uint32_t binding, std::unique_ptr<VulkanDescriptorBinding> &&vulkanDescriptorBinding);
//
//        VulkanDescriptorBindingSet &updateUniformBuffer(const VulkanCommandPool& commandPool, uint32_t binding, const void *data, uint32_t size);
    };

} // engine
