//
// Created by leixing on 2025/4/24.
//

#include "VertexBufferDescription.h"

namespace engine {

    VertexBufferDescription::VertexBufferDescription(std::vector<vk::VertexInputBindingDescription> &&vertexInputBindingDescriptions,
                                                     std::vector<vk::VertexInputAttributeDescription> &&vertexInputAttributeDescriptions)
            : mVertexInputBindingDescriptions(std::move(vertexInputBindingDescriptions)),
              mVertexInputAttributeDescriptions(std::move(vertexInputAttributeDescriptions)) {}

    VertexBufferDescription::~VertexBufferDescription() = default;


    const std::vector<vk::VertexInputBindingDescription> &VertexBufferDescription::getVertexInputBindingDescriptions() const {
        return mVertexInputBindingDescriptions;
    }


    const std::vector<vk::VertexInputAttributeDescription> &VertexBufferDescription::getVertexInputAttributeDescriptions() const {
        return mVertexInputAttributeDescriptions;
    }

} // engine