//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/instance/VulkanInstance.h"
#include "vklite/instance/VulkanInstancePlugin.h"

namespace vklite {

    class VulkanAndroidInstancePlugin : public VulkanInstancePlugin {

    public:
        VulkanAndroidInstancePlugin();

        ~VulkanAndroidInstancePlugin() override;

        void onVulkanInstanceCreated(const VulkanInstance &vulkanInstance) override;
    };

} // engine
