//
// Created by leixing on 2025/4/24.
//

#include "VertexBufferDescription.h"

namespace vklite {

    VertexBufferDescription::VertexBufferDescription(std::vector<vk::VertexInputBindingDescription> &&vertexInputBindingDescriptions,
                                                     std::vector<vk::VertexInputAttributeDescription> &&vertexInputAttributeDescriptions)
            : mVertexInputBindingDescriptions(std::move(vertexInputBindingDescriptions)),
              mVertexInputAttributeDescriptions(std::move(vertexInputAttributeDescriptions)) {}

    VertexBufferDescription::VertexBufferDescription(VertexBufferDescription &&other) noexcept
            : mVertexInputBindingDescriptions(std::move(other.mVertexInputBindingDescriptions)),
              mVertexInputAttributeDescriptions(std::move(other.mVertexInputAttributeDescriptions)) {}

    VertexBufferDescription::~VertexBufferDescription() = default;

    VertexBufferDescription &VertexBufferDescription::operator=(VertexBufferDescription &&other) noexcept {
        if (this != &other) {
            mVertexInputBindingDescriptions = std::move(other.mVertexInputBindingDescriptions);
            mVertexInputAttributeDescriptions = std::move(other.mVertexInputAttributeDescriptions);
        }
        return *this;
    }

    const std::vector<vk::VertexInputBindingDescription> &VertexBufferDescription::getVertexInputBindingDescriptions() const {
        return mVertexInputBindingDescriptions;
    }


    const std::vector<vk::VertexInputAttributeDescription> &VertexBufferDescription::getVertexInputAttributeDescriptions() const {
        return mVertexInputAttributeDescriptions;
    }

} // engine