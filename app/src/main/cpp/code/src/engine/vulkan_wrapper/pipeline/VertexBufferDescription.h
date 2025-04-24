//
// Created by leixing on 2025/4/24.
//

#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalVertexBuffer.h"

namespace engine {

    class VertexBufferDescription {
    private:
        std::vector<vk::VertexInputBindingDescription> mVertexInputBindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> mVertexInputAttributeDescriptions;

    public:

        VertexBufferDescription();

        ~VertexBufferDescription();

        [[nodiscard]]
        const std::vector<vk::VertexInputBindingDescription> &getVertexInputBindingDescriptions() const;

        [[nodiscard]]
        const std::vector<vk::VertexInputAttributeDescription> &getVertexInputAttributeDescriptions() const;

    };

} // engine
