//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "engine/vulkan_wrapper/image/VulkanSamplerBuilderInterface.h"
#include "engine/vulkan_wrapper/image/VulkanImageInterface.h"

namespace engine {

    class VulkanSamplerBuilder : public VulkanSamplerBuilderInterface {
    private:

    public:

        VulkanSamplerBuilder();

        ~VulkanSamplerBuilder() override;

        std::unique_ptr<VulkanSamplerInterface> build(const VulkanDevice &vulkanDevice, const VulkanImageInterface& image) override;

    };

} // engine
