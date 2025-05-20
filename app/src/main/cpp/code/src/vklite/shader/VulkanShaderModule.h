//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include "vklite/device/VulkanDevice.h"

namespace vklite {

    class VulkanShaderModule {
        const VulkanDevice &mVulkanDevice;

        vk::ShaderModule mShaderModule;

    public:
        VulkanShaderModule(const VulkanDevice &vulkanDevice, const std::vector<char> &code);

        ~VulkanShaderModule();

        const vk::ShaderModule &getShaderModule() const;
    };

} // engine
