//
// Created by leixing on 2025/4/24.
//

#include "VertexBufferDescription.h"

namespace engine {

    VertexBufferDescription::VertexBufferDescription() = default;

    VertexBufferDescription::~VertexBufferDescription() = default;


    const std::vector<vk::VertexInputBindingDescription> &VertexBufferDescription::getVertexInputBindingDescriptions() const {
        return mVertexInputBindingDescriptions;
    }


    const std::vector<vk::VertexInputAttributeDescription> &VertexBufferDescription::getVertexInputAttributeDescriptions() const {
        return mVertexInputAttributeDescriptions;
    }

} // engine