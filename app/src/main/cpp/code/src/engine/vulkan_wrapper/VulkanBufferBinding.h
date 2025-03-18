//
// Created by leixing on 2025/3/18.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/buffer/VulkanBufferView.h"
#include "engine/VulkanDescriptorConfigure.h"

namespace engine {

    class VulkanBufferBinding {
    private:
        const vk::DescriptorBufferInfo &mDescriptorBufferInfo;
        const VulkanDescriptorConfigure &mVulkanDescriptorConfigure;

    public:
        VulkanBufferBinding(const vk::DescriptorBufferInfo &descriptorBufferInfo,
                            const VulkanDescriptorConfigure &vulkanDescriptorConfigure);

        ~VulkanBufferBinding();
    };

} // engine
