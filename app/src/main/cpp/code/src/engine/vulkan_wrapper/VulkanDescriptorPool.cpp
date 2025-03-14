//
// Created by leixing on 2025/3/12.
//

#include "VulkanDescriptorPool.h"

namespace engine {

    VulkanDescriptorPool::VulkanDescriptorPool(const VulkanDevice &vulkanDevice, std::vector<vk::DescriptorPoolSize> descriptorPoolSizes, uint32_t maxSets)
            : mVulkanDevice(vulkanDevice) {

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo
                .setPoolSizes(descriptorPoolSizes)
                .setMaxSets(maxSets)
                .setFlags(vk::DescriptorPoolCreateFlags{});

        mDescriptorPool = mVulkanDevice.getDevice().createDescriptorPool(descriptorPoolCreateInfo);
    }

    VulkanDescriptorPool::~VulkanDescriptorPool() {
        mVulkanDevice.getDevice().destroy(mDescriptorPool);
    }

    const vk::DescriptorPool &VulkanDescriptorPool::getDescriptorPool() const {
        return mDescriptorPool;
    }

    std::vector<vk::DescriptorSet> VulkanDescriptorPool::allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
        vk::DescriptorSetAllocateInfo allocateInfo{};
        allocateInfo
                .setDescriptorPool(mDescriptorPool)
                .setSetLayouts(descriptorSetLayouts);

        return mVulkanDevice.getDevice().allocateDescriptorSets(allocateInfo);
    }

} // engine