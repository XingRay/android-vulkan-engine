//
// Created by leixing on 2025/3/11.
//

#include "VertexConfigures.h"

namespace engine {
    VertexConfigures::VertexConfigures() {

    }

    VertexConfigures::~VertexConfigures() {

    }

    VertexConfigures &VertexConfigures::addVertexConfigure(VertexConfigure vertexConfigure){
        mVertexConfigures.push_back(vertexConfigure);
        return *this;
    }

    std::vector<vk::VertexInputBindingDescription> VertexConfigures::createVertexInputBindingDescriptions() const {
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions;

        for (const VertexConfigure &vertexConfigure: mVertexConfigures) {
            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription
                    .setBinding(vertexConfigure.getBinding())
                    .setStride(vertexConfigure.getStride())
                    .setInputRate(vk::VertexInputRate::eVertex);
            vertexInputBindingDescriptions.push_back(bindingDescription);
        }

        return vertexInputBindingDescriptions;
    }


    std::vector<vk::VertexInputAttributeDescription> VertexConfigures::createVertexInputAttributeDescriptions() const {

        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;

        for (const VertexConfigure &vertexConfigure: mVertexConfigures) {
            for (const VertexAttributeConfigure &attributeConfigure: vertexConfigure.getAttributes()) {
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

    std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> VertexConfigures::createVertexBuffers(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> vertexBuffers{};

        for (const VertexConfigure &configure: mVertexConfigures) {
            vertexBuffers.push_back(configure.createVertexBuffer(vulkanDevice, commandPool));
        }

        return vertexBuffers;
    }

} // engine