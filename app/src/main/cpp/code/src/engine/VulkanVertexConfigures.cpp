//
// Created by leixing on 2025/3/11.
//

#include "engine/VulkanVertexConfigures.h"

namespace engine {
    VulkanVertexConfigures::VulkanVertexConfigures() {

    }

    VulkanVertexConfigures::~VulkanVertexConfigures() {

    }

    VulkanVertexConfigures &VulkanVertexConfigures::addVulkanVertexConfigure(VulkanVertexConfigure vulkanVertexConfigure) {
        mVulkanVertexConfigures.push_back(vulkanVertexConfigure);
        return *this;
    }

    std::vector<vk::VertexInputBindingDescription> VulkanVertexConfigures::createVertexInputBindingDescriptions() const {
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions;

        for (const VulkanVertexConfigure &vertexConfigure: mVulkanVertexConfigures) {
            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription
                    .setBinding(vertexConfigure.getBinding())
                    .setStride(vertexConfigure.getStride())
                    .setInputRate(vk::VertexInputRate::eVertex);
            vertexInputBindingDescriptions.push_back(bindingDescription);
        }

        return vertexInputBindingDescriptions;
    }


    std::vector<vk::VertexInputAttributeDescription> VulkanVertexConfigures::createVertexInputAttributeDescriptions() const {

        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;

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

    std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> VulkanVertexConfigures::createVertexBuffers(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> vertexBuffers{};

        for (const VulkanVertexConfigure &configure: mVulkanVertexConfigures) {
            vertexBuffers.push_back(configure.createVertexBuffer(vulkanDevice, commandPool));
        }

        return vertexBuffers;
    }

} // engine