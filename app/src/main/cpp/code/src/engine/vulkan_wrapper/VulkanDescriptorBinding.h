//
// Created by leixing on 2025/3/18.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/buffer/VulkanBufferView.h"
#include "engine/VulkanDescriptorConfigure.h"

namespace engine {

    class VulkanDescriptorBinding {
    private:


    public:
        VulkanDescriptorBinding();

        ~VulkanDescriptorBinding();

        virtual vk::WriteDescriptorSet createWriteDescriptorSet() const = 0;
    };

} // engine
