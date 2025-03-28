//
// Created by leixing on 2025/3/18.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanBufferView.h"
#include "engine/VulkanDescriptorConfigure.h"

namespace engine {

    class VulkanDescriptorBinding {
    private:


    public:
        VulkanDescriptorBinding();

        ~VulkanDescriptorBinding();

        [[nodiscard]]
        virtual vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorSet& descriptorSet) const = 0;
    };

} // engine
