//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "engine/vulkan_wrapper/VulkanInstance.h"

namespace engine {

    class VulkanInstancePlugin {

    public:
        VulkanInstancePlugin() ;

        virtual ~VulkanInstancePlugin();

        virtual void onVulkanInstanceCreated(const VulkanInstance &vulkanInstance) = 0;
    };

} // engine
