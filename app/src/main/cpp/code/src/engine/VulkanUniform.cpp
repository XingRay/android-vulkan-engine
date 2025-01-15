//
// Created by leixing on 2025/1/14.
//

#include "engine/VulkanUniform.h"

namespace engine {

    vk::DescriptorType VulkanNormalUniform::getDescriptorType() {
        return vk::DescriptorType::eUniformBuffer;
    }

    vk::DescriptorType VulkanSamplerUniform::getDescriptorType() {
        return vk::DescriptorType::eCombinedImageSampler;
    }
}