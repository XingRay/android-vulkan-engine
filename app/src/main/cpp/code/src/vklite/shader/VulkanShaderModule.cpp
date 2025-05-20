//
// Created by leixing on 2025/3/10.
//

#include "VulkanShaderModule.h"

namespace vklite {
    VulkanShaderModule::VulkanShaderModule(const VulkanDevice &vulkanDevice, const std::vector<char> &code)
            : mVulkanDevice(vulkanDevice) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCodeSize(code.size())
                .setPCode(reinterpret_cast<const uint32_t *>(code.data()));

        mShaderModule = vulkanDevice.getDevice().createShaderModule(createInfo);
    }

    VulkanShaderModule::~VulkanShaderModule() {
        mVulkanDevice.getDevice().destroy(mShaderModule);
    }

    const vk::ShaderModule &VulkanShaderModule::getShaderModule() const {
        return mShaderModule;
    }

} // engine