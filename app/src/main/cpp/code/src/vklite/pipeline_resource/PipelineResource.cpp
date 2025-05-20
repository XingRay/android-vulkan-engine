//
// Created by leixing on 2025/4/18.
//

#include "PipelineResource.h"

namespace vklite {

    PipelineResource::PipelineResource() {

    }

    PipelineResource::~PipelineResource() {

    }

    const std::vector<vk::Buffer> &PipelineResource::getVertexBuffers() const {
        return mVertexBuffers;
    }

    const std::vector<vk::DeviceSize> &PipelineResource::getVertexBufferOffsets() const {
        return mVertexBufferOffsets;
    }

    const vk::Buffer &PipelineResource::getIndexBuffer() const {
        return mIndexBuffer;
    }

    uint32_t PipelineResource::getIndicesCount() const {
        return mIndicesCount;
    }

    const std::vector<vk::DescriptorSet> &PipelineResource::getDescriptorSets() const {
        return mDescriptorSets;
    }

    const std::vector<PushConstant> &PipelineResource::getPushConstants() const {
        return mPushConstants;
    }

//    void PipelineResource::updateDescriptorSets(std::vector<vk::WriteDescriptorSet>& writeDescriptorSets) {
//        for (const auto &setsEntry: vulkanDescriptorBindingSetOfFrame->getVulkanDescriptorBindingSets()) {
//            uint32_t set = setsEntry.first;
//            const std::unique_ptr<VulkanDescriptorBindingSet> &vulkanDescriptorBindingSet = setsEntry.second;
//
//            for (const auto &bindingEntry: vulkanDescriptorBindingSet->getVulkanDescriptorBindings()) {
//                uint32_t binding = bindingEntry.first;
//                const std::unique_ptr<VulkanDescriptorBinding> &vulkanDescriptorBinding = bindingEntry.second;
//                vk::DescriptorType type = vulkanDescriptorBinding->getDescriptorType();
//                if (type == vk::DescriptorType::eUniformBuffer) {
//                    const std::unique_ptr<VulkanDescriptorBufferInfo> &bufferInfo = vulkanDescriptorBinding->getVulkanDescriptorBufferInfo();
//                    if (bufferInfo != nullptr) {
//                        vk::WriteDescriptorSet writeDescriptorSet{};
//
//                        vk::DescriptorBufferInfo descriptorBufferInfo = bufferInfo->createDescriptorBufferInfo();
//                        std::array<vk::DescriptorBufferInfo, 1> descriptorBufferInfos = {descriptorBufferInfo};
//
//                        writeDescriptorSet
//                                .setDstSet(mDescriptorSets[frameIndex][set])
//                                .setDstBinding(binding)
//                                .setDstArrayElement(vulkanDescriptorBinding->getDescriptorOffset())
//                                .setDescriptorCount(vulkanDescriptorBinding->getDescriptorRange())
//                                .setDescriptorType(vulkanDescriptorBinding->getDescriptorType())
//                                .setBufferInfo(descriptorBufferInfos);
//                        writeDescriptorSets.push_back(writeDescriptorSet);
//                    }
//                } else if (type == vk::DescriptorType::eCombinedImageSampler) {
//                    const std::unique_ptr<VulkanDescriptorImageInfo> &imageInfo = vulkanDescriptorBinding->getVulkanDescriptorImageInfo();
//                    if (imageInfo != nullptr) {
//                        vk::WriteDescriptorSet writeDescriptorSet{};
//
//                        vk::DescriptorImageInfo descriptorImageInfo = imageInfo->createDescriptorImageInfo();
//                        std::array<vk::DescriptorImageInfo, 1> descriptorImageInfos = {descriptorImageInfo};
//
//                        writeDescriptorSet
//                                .setDstSet(mDescriptorSets[frameIndex][set])
//                                .setDstBinding(binding)
//                                        // 当描述符是一个数组时，这个属性指定从数组的哪个元素开始更新
//                                .setDstArrayElement(vulkanDescriptorBinding->getDescriptorOffset())
//                                .setDescriptorCount(vulkanDescriptorBinding->getDescriptorRange())
//                                .setDescriptorType(vulkanDescriptorBinding->getDescriptorType())
//                                .setImageInfo(descriptorImageInfos);
//                        writeDescriptorSets.push_back(writeDescriptorSet);
//                    }
//                } else {
//                    throw std::runtime_error("unknown descriptor type");
//                }
//
//            }
//        }
//
//        if (!writeDescriptorSets.empty()) {
//            mVulkanDevice.getDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
//        }
//    }

} // engine