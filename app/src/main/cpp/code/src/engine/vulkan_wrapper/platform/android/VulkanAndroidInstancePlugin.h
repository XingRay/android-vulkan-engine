//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanInstancePlugin.h"

namespace engine {

    class VulkanAndroidInstancePlugin : public VulkanInstancePlugin {

    public:
        VulkanAndroidInstancePlugin();

        ~VulkanAndroidInstancePlugin() override;

        void onVulkanInstanceCreated(const VulkanInstance &vulkanInstance) override;
    };

} // engine
