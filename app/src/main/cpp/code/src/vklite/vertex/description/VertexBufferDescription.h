//
// Created by leixing on 2025/4/24.
//

#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"

#include "vklite/buffer/device_local/VulkanDeviceLocalVertexBuffer.h"

namespace vklite {

    class VertexBufferDescription {
    private:
        std::vector<vk::VertexInputBindingDescription> mVertexInputBindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> mVertexInputAttributeDescriptions;

    public:

        VertexBufferDescription(std::vector<vk::VertexInputBindingDescription> &&vertexInputBindingDescriptions,
                                std::vector<vk::VertexInputAttributeDescription> &&vertexInputAttributeDescriptions);

        VertexBufferDescription(VertexBufferDescription &&other) noexcept;

        ~VertexBufferDescription();

        VertexBufferDescription &operator=(VertexBufferDescription &&other) noexcept;

        [[nodiscard]]
        const std::vector<vk::VertexInputBindingDescription> &getVertexInputBindingDescriptions() const;

        [[nodiscard]]
        const std::vector<vk::VertexInputAttributeDescription> &getVertexInputAttributeDescriptions() const;

    };

} // engine
