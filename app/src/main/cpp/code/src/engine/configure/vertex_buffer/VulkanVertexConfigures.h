//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>

#include "VulkanVertexConfigure.h"

namespace engine {

    class VulkanVertexConfigures {
    private:
        /**
         * vertex
         */
        std::vector<VulkanVertexConfigure> mVulkanVertexConfigures;

    public:
        VulkanVertexConfigures();

        ~VulkanVertexConfigures();

        VulkanVertexConfigures &addVulkanVertexConfigure(VulkanVertexConfigure vulkanVertexConfigure);

        [[nodiscard]]
        std::vector<vk::VertexInputBindingDescription> createVertexInputBindingDescriptions() const;

        [[nodiscard]]
        std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions() const;

        [[nodiscard]]
        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> createVertexBuffers(const VulkanDevice& vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
