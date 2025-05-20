//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "VulkanImageBuilderInterface.h"

namespace vklite {

    class VulkanImageBuilder : public VulkanImageBuilderInterface {
    private:

    public:
        VulkanImageBuilder();

        ~VulkanImageBuilder() override;

        [[nodiscard]]
        std::unique_ptr<VulkanImageInterface> build(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, vk::Format format) override;
    };

} // engine
