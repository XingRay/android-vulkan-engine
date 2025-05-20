//
// Created by leixing on 2025/3/11.
//

#include "VertexConfigure.h"

namespace vklite {

    VertexConfigure::VertexConfigure() {

    }

    VertexConfigure::~VertexConfigure() {

    }

    VertexConfigure &VertexConfigure::add(VertexBindingConfigure &&vertexConfigure) {
        mVertexBindingConfigures.emplace(vertexConfigure.getBinding(), std::move(vertexConfigure));
        return *this;
    }

    std::vector<vk::VertexInputBindingDescription> VertexConfigure::createVertexInputBindingDescriptions() const {
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions;

        for (const auto &entry: mVertexBindingConfigures) {
            uint32_t binding = entry.first;
            const VertexBindingConfigure &vertexConfigure = entry.second;

            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription
                    .setBinding(binding)
                    .setStride(vertexConfigure.getStride())
                    .setInputRate(vk::VertexInputRate::eVertex);
            vertexInputBindingDescriptions.push_back(bindingDescription);
        }

        return vertexInputBindingDescriptions;
    }


    std::vector<vk::VertexInputAttributeDescription> VertexConfigure::createVertexInputAttributeDescriptions() const {

        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;

        for (const auto &bindingEntry: mVertexBindingConfigures) {
            uint32_t binding = bindingEntry.first;
            const VertexBindingConfigure &vertexBindingConfigure = bindingEntry.second;

            for (const auto &attributeEntry: vertexBindingConfigure.getAttributes()) {
                uint32_t location = attributeEntry.first;
                const VertexAttributeConfigure &attributeConfigure = attributeEntry.second;

                vk::VertexInputAttributeDescription attributeDescription{};
                attributeDescription
                        .setBinding(binding)
                        .setLocation(location)
                        .setFormat(attributeConfigure.getFormat())
                        .setOffset(attributeConfigure.getOffset());

                vertexInputAttributeDescriptions.push_back(attributeDescription);
            }
        }

        return vertexInputAttributeDescriptions;
    }

//    std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> VertexConfigure::createVertexBuffers(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const {
//        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> vertexBuffers{};
//
//        for (const VertexConfigure &configure: mVertexConfigures) {
//            vertexBuffers.push_back(configure.createVertexBuffer(vulkanDevice, commandPool));
//        }
//
//        return vertexBuffers;
//    }

    std::unique_ptr<VertexBufferDescription> VertexConfigure::createVertexBufferDescription() const {
        return std::make_unique<VertexBufferDescription>(createVertexInputBindingDescriptions(),
                                                         createVertexInputAttributeDescriptions());
    }

} // engine