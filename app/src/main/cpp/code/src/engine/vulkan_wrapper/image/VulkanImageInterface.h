//
// Created by leixing on 2025/3/30.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanImageInterface {
    private:

    public:
        VulkanImageInterface();

        virtual ~VulkanImageInterface() = 0;

        [[nodiscard]]
        virtual const vk::Image &getImage() const = 0;

        [[nodiscard]]
        virtual const vk::ImageView &getImageView() const = 0;

        [[nodiscard]]
        virtual uint32_t getMipLevels() const = 0;
    };

} // engine
