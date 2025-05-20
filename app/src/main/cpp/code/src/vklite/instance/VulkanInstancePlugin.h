//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "VulkanInstance.h"

namespace vklite {

    class VulkanInstancePlugin {

    public:
        VulkanInstancePlugin() ;

        virtual ~VulkanInstancePlugin();

        virtual void onVulkanInstanceCreated(const VulkanInstance &vulkanInstance) = 0;
    };

} // engine
