//
// Created by leixing on 2024/12/28.
//

#include "VulkanDescriptorSet.h"
#include "VulkanUniformBuffer.h"
#include "VulkanTextureSampler.h"

#include "engine/Log.h"

namespace engine {
    VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice &device,
                                             uint32_t frameCount,
                                             const VulkanShader &shader,
                                             const std::vector<std::vector<std::unique_ptr<VulkanUniformBuffer>>> &uniformBuffers,
                                             const std::vector<std::vector<std::unique_ptr<VulkanTextureSampler>>> &textureSamplers)
            : mDevice(device) {

        LOG_D("VulkanDescriptorSet::VulkanDescriptorSet");
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes = shader.getUniformDescriptorPoolSizes();

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo
                .setPoolSizes(descriptorPoolSizes)
                .setMaxSets(frameCount)
                .setFlags(vk::DescriptorPoolCreateFlags{});

        LOG_D("create DescriptorPool");
        mDescriptorPool = mDevice.getDevice().createDescriptorPool(descriptorPoolCreateInfo);


        LOG_D("descriptorSetLayoutBindings");
        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = shader.getUniformDescriptorSetLayoutBindings();

        LOG_D("create DescriptorSetLayout");
        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
        descriptorSetLayoutCreateInfo
                .setBindings(descriptorSetLayoutBindings);

        mDescriptorSetLayout = mDevice.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

        LOG_D("allocate DescriptorSets");
        // createDescriptorSets
        // 大小为 imageCount，并且每个元素都初始化为 mDescriptorSetLayout
        std::vector<vk::DescriptorSetLayout> layouts(frameCount, mDescriptorSetLayout);
        vk::DescriptorSetAllocateInfo allocateInfo{};
        allocateInfo.setDescriptorPool(mDescriptorPool)
                .setSetLayouts(layouts);
        mDescriptorSets = mDevice.getDevice().allocateDescriptorSets(allocateInfo);

        LOG_D("update DescriptorSets");
        const std::vector<uint32_t> &vertexUniformSizes = shader.getUniformSizes();
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
                        .setDstBinding(j)      // binding // todo merge with sampler
                        .setDstArrayElement(0)
                        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                        .setDescriptorCount(1)
                        .setPBufferInfo(&descriptorBufferInfo)
                        .setPImageInfo(nullptr)
                        .setPTexelBufferView(nullptr);

                writeDescriptorSets.push_back(writeDescriptorSet);
            }

            const std::vector<std::unique_ptr<VulkanTextureSampler>> &textureSamplersOfFrame = textureSamplers[i];
            for (int j = 0; j < textureSamplersOfFrame.size(); j++) {
                const auto &textureSampler = textureSamplersOfFrame[j];

                LOG_D("samplerDescriptorImageInfo: {vk::ImageLayout::eShaderReadOnlyOptimal, %p, %p}",
                      &textureSampler->getTextureImageView(),
                      &textureSampler->getTextureSampler());
                vk::DescriptorImageInfo samplerDescriptorImageInfo;
                samplerDescriptorImageInfo
                        .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                        .setImageView(textureSampler->getTextureImageView())
                        .setSampler(textureSampler->getTextureSampler());

                LOG_D("samplerWriteDescriptorSet: {mDescriptorSets[%d], setDstBinding:%d, DstArrayElement:0, DescriptorType:eCombinedImageSampler, DescriptorCount:1}", i, j);
                vk::WriteDescriptorSet samplerWriteDescriptorSet{};
                samplerWriteDescriptorSet
                        .setDstSet(mDescriptorSets[i])
                        .setDstBinding(j) // todo merge with normal uniform
                        .setDstArrayElement(0)
                        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                        .setDescriptorCount(1)
                                // 下面根据 DescriptorType 3 选 1 设置， 这里的描述符基于缓冲区，所以选择使用 pBufferInfo。
                                // pBufferInfo 字段用于引用缓冲区数据的描述符
                        .setPBufferInfo(nullptr)
                                // pImageInfo 用于引用图像数据的描述符
                        .setPImageInfo(&samplerDescriptorImageInfo)
                                // pTexelBufferView 用于引用缓冲区视图的描述符
                        .setPTexelBufferView(nullptr);

                writeDescriptorSets.push_back(samplerWriteDescriptorSet);
            }

            // 更新描述符集
            mDevice.getDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
        }
        LOG_D("VulkanDescriptorSet::VulkanDescriptorSet end");
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
