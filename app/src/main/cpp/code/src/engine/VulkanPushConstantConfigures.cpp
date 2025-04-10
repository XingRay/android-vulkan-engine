//
// Created by leixing on 2025/3/11.
//

#include "engine/VulkanPushConstantConfigures.h"

namespace engine {
    VulkanPushConstantConfigures::VulkanPushConstantConfigures() {

    }

    VulkanPushConstantConfigures::~VulkanPushConstantConfigures() {

    }

    VulkanPushConstantConfigures &VulkanPushConstantConfigures::addVulkanPushConstantConfigures(VulkanPushConstantConfigure vulkanPushConstantConfigure) {
        mVulkanPushConstantConfigures.push_back(vulkanPushConstantConfigure);
        return *this;
    }

    std::vector<vk::PushConstantRange> VulkanPushConstantConfigures::createPushConstantRanges() const {
        // push constant
        std::vector<vk::PushConstantRange> pushConstantRanges;
        for (const VulkanPushConstantConfigure &pushConstantConfigure: mVulkanPushConstantConfigures) {
            vk::PushConstantRange pushConstantRange = pushConstantConfigure.createPushConstantRange();
            pushConstantRanges.push_back(pushConstantRange);
        }

        return pushConstantRanges;
    }

    uint32_t VulkanPushConstantConfigures::calcTotalPushConstantSize() const {
        uint32_t totalPushConstantSize = 0;
        for (const VulkanPushConstantConfigure &pushConstantConfigure: mVulkanPushConstantConfigures) {
            totalPushConstantSize += pushConstantConfigure.getSize();
        }

        return totalPushConstantSize;
    }
} // engine