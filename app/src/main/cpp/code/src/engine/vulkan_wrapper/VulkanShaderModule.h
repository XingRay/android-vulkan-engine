//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    class VulkanShaderModule {
        const VulkanDevice &mVulkanDevice;

        vk::ShaderModule mShaderModule;

    public:
        VulkanShaderModule(const VulkanDevice &vulkanDevice, const std::vector<char> &code);

        ~VulkanShaderModule();

        const vk::ShaderModule &getShaderModule() const;
    };

} // engine
