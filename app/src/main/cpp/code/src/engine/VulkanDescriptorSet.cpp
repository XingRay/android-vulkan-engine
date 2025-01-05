//
// Created by leixing on 2024/12/28.
//

#include "VulkanDescriptorSet.h"
#include "VulkanUniformBuffer.h"

#include "Log.h"

namespace engine {
    VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice &device,
                                             uint32_t frameCount,
                                             const VulkanVertexShader &vertexShader,
                                             const VulkanFragmentShader &fragmentShader,
                                             const std::vector<std::vector<std::unique_ptr<VulkanUniformBuffer>>> &uniformBuffers)
            : mDevice(device) {
        LOG_D("VulkanDescriptorSet, size:%ld", fragmentShader.getShaderCode().size());

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo
                .setPoolSizes(vertexShader.getUniformDescriptorPoolSizes())
                .setMaxSets(frameCount)
//            .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);
                .setFlags(vk::DescriptorPoolCreateFlags{});

        mDescriptorPool = mDevice.getDevice().createDescriptorPool(descriptorPoolCreateInfo);

        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
        descriptorSetLayoutCreateInfo
                .setBindings(vertexShader.getUniformDescriptorSetLayoutBindings());

        mDescriptorSetLayout = mDevice.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

        // createDescriptorSets
        // 大小为 imageCount，并且每个元素都初始化为 mDescriptorSetLayout
        std::vector<vk::DescriptorSetLayout> layouts(frameCount, mDescriptorSetLayout);

        vk::DescriptorSetAllocateInfo allocateInfo{};
        allocateInfo.setDescriptorPool(mDescriptorPool)
                .setSetLayouts(layouts);

        mDescriptorSets = mDevice.getDevice().allocateDescriptorSets(allocateInfo);
        const std::vector<uint32_t> &vertexUniformSizes = vertexShader.getUniformSizes();

        for (int i = 0; i < uniformBuffers.size(); i++) {
            std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
            const std::vector<std::unique_ptr<VulkanUniformBuffer>> &uniformBuffersOfFrame = uniformBuffers[i];
            for (int j = 0; j < uniformBuffersOfFrame.size(); j++) {
                const auto &uniformBuffer = uniformBuffersOfFrame[j];
                vk::DescriptorBufferInfo descriptorBufferInfo{};
                descriptorBufferInfo
                        .setBuffer(uniformBuffer->getUniformBuffer())
                        .setOffset(0)
                        .setRange(vertexUniformSizes[j]);

                vk::WriteDescriptorSet writeDescriptorSet{};
                writeDescriptorSet.setDstSet(mDescriptorSets[i])
                        .setDstBinding(j)      // binding
                        .setDstArrayElement(0)
                        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                        .setDescriptorCount(1)
                        .setPBufferInfo(&descriptorBufferInfo)
                        .setPImageInfo(nullptr)
                        .setPTexelBufferView(nullptr);

                writeDescriptorSets.push_back(writeDescriptorSet);
            }

            // 更新描述符集
            mDevice.getDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
        }
    }

    VulkanDescriptorSet::~VulkanDescriptorSet() {
        LOG_D("VulkanDescriptorSet::~VulkanDescriptorSet");
        const vk::Device device = mDevice.getDevice();
        device.destroyDescriptorPool(mDescriptorPool);
        device.destroyDescriptorSetLayout(mDescriptorSetLayout);
    }

    const vk::DescriptorSetLayout &VulkanDescriptorSet::getDescriptorSetLayout() const {
        return mDescriptorSetLayout;
    }

    const vk::DescriptorPool &VulkanDescriptorSet::getDescriptorPool() const {
        return mDescriptorPool;
    }

    const std::vector<vk::DescriptorSet> &VulkanDescriptorSet::getDescriptorSets() const {
        return mDescriptorSets;
    }
}
