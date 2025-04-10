//
// Created by leixing on 2025/4/8.
//

#include "engine/VulkanDescriptorImageInfoConfigure.h"

namespace engine {

    VulkanDescriptorImageInfoConfigure::VulkanDescriptorImageInfoConfigure(std::unique_ptr <engine::ImageInterface> &&image)
            : mImage(std::move(image)) {}

    VulkanDescriptorImageInfoConfigure::~VulkanDescriptorImageInfoConfigure() {

    }
} // engine