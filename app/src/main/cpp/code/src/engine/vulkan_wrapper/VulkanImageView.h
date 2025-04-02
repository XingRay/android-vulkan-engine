//
// Created by leixing on 2025/3/29.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanImageInterface.h"
#include "engine/vulkan_wrapper/VulkanSamplerInterface.h"

namespace engine {

    // interface
    class VulkanImageView {
    private:
        std::shared_ptr<VulkanImageInterface> mVulkanImage;
        std::shared_ptr<VulkanSamplerInterface> mVulkanSampler;

    public:
        VulkanImageView();

        virtual ~VulkanImageView();

        [[nodiscard]]
        const std::shared_ptr<VulkanImageInterface> &getVulkanImage() const;

        [[nodiscard]]
        const std::shared_ptr<VulkanSamplerInterface> &getVulkanSampler() const;
    };

} // engine
