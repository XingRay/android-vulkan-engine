//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/sampler/builder/SamplerBuilderInterface.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferYcbcrConversion.h"

namespace engine {

    class VulkanAndroidHardwareBufferSamplerBuilder : public SamplerBuilderInterface {
    private:
        const VulkanAndroidHardwareBufferYcbcrConversion &mVulkanAndroidSamplerYcbcrConversion;

    public:

        VulkanAndroidHardwareBufferSamplerBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~VulkanAndroidHardwareBufferSamplerBuilder() override;

        std::unique_ptr<SamplerInterface> build(const VulkanDevice &vulkanDevice) override;

    };

} // engine
