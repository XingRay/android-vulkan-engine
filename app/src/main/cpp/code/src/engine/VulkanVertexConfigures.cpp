//
// Created by leixing on 2025/3/11.
//

#include "VulkanVertexConfigures.h"

namespace engine {
    VulkanVertexConfigures::VulkanVertexConfigures() {

    }

    VulkanVertexConfigures::~VulkanVertexConfigures() {

    }

    VulkanVertexConfigures &VulkanVertexConfigures::addVulkanVertexConfigure(VulkanVertexConfigure vulkanVertexConfigure) {
        mVulkanVertexConfigures.push_back(vulkanVertexConfigure);
        return *this;
    }

    std::vector <vk::VertexInputBindingDescription> VulkanVertexConfigures::createVertexInputBindingDescriptions() {
        std::vector <vk::VertexInputBindingDescription> vertexInputBindingDescriptions;

        for (const VulkanVertexConfigure &vertexConfigure: mVulkanVertexConfigures) {
            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription
                    .setBinding(vertexConfigure.getBinding())
                    .setStride(vertexConfigure.getSize())
                    .setInputRate(vk::VertexInputRate::eVertex);
            vertexInputBindingDescriptions.push_back(bindingDescription);
        }

        return vertexInputBindingDescriptions;
    }


    std::vector <vk::VertexInputAttributeDescription> VulkanVertexConfigures::createVertexInputAttributeDescriptions() {

        std::vector <vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;

        for (const VulkanVertexConfigure &vertexConfigure: mVulkanVertexConfigures) {
            for (const VulkanVertexAttributeConfigure &attributeConfigure: vertexConfigure.getAttributes()) {
                vk::VertexInputAttributeDescription attributeDescription{};
                attributeDescription
                        .setBinding(attributeConfigure.getBinding())
                        .setLocation(attributeConfigure.getLocation())
                        .setFormat(attributeConfigure.getFormat())
                        .setOffset(attributeConfigure.getOffset());

                vertexInputAttributeDescriptions.push_back(attributeDescription);
            }
        }

        return vertexInputAttributeDescriptions;
    }

} // engine