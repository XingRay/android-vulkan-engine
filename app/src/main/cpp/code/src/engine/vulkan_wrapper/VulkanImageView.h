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
        std::shared_ptr<VulkanImageInterface> mImage;
        std::shared_ptr<VulkanSamplerInterface> mSampler;

    public:
        VulkanImageView();

        virtual ~VulkanImageView();

        [[nodiscard]]
        const std::shared_ptr<VulkanImageInterface> &getImage() const;

        [[nodiscard]]
        const std::shared_ptr<VulkanSamplerInterface> &getSampler() const;
    };

} // engine
