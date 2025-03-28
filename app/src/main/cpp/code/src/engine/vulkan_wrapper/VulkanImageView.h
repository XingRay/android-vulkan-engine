//
// Created by leixing on 2025/3/29.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanImageView {
    private:

    public:
        VulkanImageView();

        ~VulkanImageView();

        [[nodiscard]]
        virtual vk::ImageView getImageView() const = 0;

        [[nodiscard]]
        virtual vk::Sampler getSampler() const = 0;
    };

} // engine
