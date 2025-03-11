//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>

#include "engine/VulkanVertexConfigure.h"

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

        VulkanVertexConfigures& addVulkanVertexConfigure(VulkanVertexConfigure vulkanVertexConfigure);

        std::vector<vk::VertexInputBindingDescription> createVertexInputBindingDescriptions();

        std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions();
    };

} // engine
