//
// Created by leixing on 2025/3/12.
//

#include "DescriptorPool.h"

namespace vklite {

    DescriptorPool::DescriptorPool(const VulkanDevice &vulkanDevice, std::vector<vk::DescriptorPoolSize> descriptorPoolSizes, uint32_t maxSets)
            : mVulkanDevice(vulkanDevice) {

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo
                .setPoolSizes(descriptorPoolSizes)
                .setMaxSets(maxSets)
                .setFlags(vk::DescriptorPoolCreateFlags{});

        mDescriptorPool = mVulkanDevice.getDevice().createDescriptorPool(descriptorPoolCreateInfo);
    }

    DescriptorPool::~DescriptorPool() {
        mVulkanDevice.getDevice().destroy(mDescriptorPool);
    }

    const vk::DescriptorPool &DescriptorPool::getDescriptorPool() const {
        return mDescriptorPool;
    }

    std::vector<vk::DescriptorSet> DescriptorPool::allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
        vk::DescriptorSetAllocateInfo allocateInfo{};
        allocateInfo
                .setDescriptorPool(mDescriptorPool)
                .setSetLayouts(descriptorSetLayouts);

        return mVulkanDevice.getDevice().allocateDescriptorSets(allocateInfo);
    }

} // engine