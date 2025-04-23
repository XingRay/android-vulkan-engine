//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>

#include "VertexConfigure.h"

namespace engine {

    class VulkanVertexConfigures {
    private:
        /**
         * vertex
         */
        std::vector<VertexConfigure> mVertexConfigures;

    public:
        VulkanVertexConfigures();

        ~VulkanVertexConfigures();

        VulkanVertexConfigures &addVertexConfigure(VertexConfigure vertexConfigure);

        [[nodiscard]]
        std::vector<vk::VertexInputBindingDescription> createVertexInputBindingDescriptions() const;

        [[nodiscard]]
        std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions() const;

        [[nodiscard]]
        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> createVertexBuffers(const VulkanDevice& vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
