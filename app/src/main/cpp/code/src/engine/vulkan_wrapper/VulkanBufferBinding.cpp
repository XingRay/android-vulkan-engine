//
// Created by leixing on 2025/3/18.
//

#include "VulkanBufferBinding.h"

namespace engine {

    VulkanBufferBinding::VulkanBufferBinding(const vk::DescriptorBufferInfo &descriptorBufferInfo,
                                             const VulkanDescriptorConfigure &vulkanDescriptorConfigure)
            : mDescriptorBufferInfo(descriptorBufferInfo), mVulkanDescriptorConfigure(vulkanDescriptorConfigure) {

    }

    VulkanBufferBinding::~VulkanBufferBinding() = default;

//    vk::WriteDescriptorSet VulkanBufferBinding::createWriteDescriptorSet(vk::Buffer) {
//
//
//        std::array<vk::DescriptorBufferInfo, 1> descriptorBufferInfos = {
//                mDescriptorBufferInfo
//        };
//
//        vk::WriteDescriptorSet writeDescriptorSet{};
//        writeDescriptorSet
//                .setBufferInfo(descriptorBufferInfos)
//                .setDstSet(mDescriptorSets[frameIndex][setIndex])
//                .setDstBinding(pUniformBuffer->getBinding())
//                .setDstArrayElement(pUniformBuffer->getIndex())
//                .setDescriptorCount(1)
//                .setDescriptorType(vk::DescriptorType::eUniformBuffer);
//    }

} // engine