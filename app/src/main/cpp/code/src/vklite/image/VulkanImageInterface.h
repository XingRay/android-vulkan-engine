//
// Created by leixing on 2025/3/30.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/command/VulkanCommandPool.h"

namespace vklite {

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

        virtual void transitionImageLayout(const VulkanCommandPool &commandPool) = 0;

        virtual void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) = 0;
    };

} // engine
