//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <vector>

#include "engine/VulkanPushConstantConfigure.h"

namespace engine {

    class VulkanPushConstantConfigures {
    private:
        std::vector<VulkanPushConstantConfigure> mVulkanPushConstantConfigures;

    public:
        VulkanPushConstantConfigures();

        ~VulkanPushConstantConfigures();

        VulkanPushConstantConfigures& addVulkanPushConstantConfigures(VulkanPushConstantConfigure vulkanPushConstantConfigure);

        std::vector<vk::PushConstantRange> createPushConstantRanges() const;

        uint32_t calcTotalPushConstantSize() const;
    };

} // engine
