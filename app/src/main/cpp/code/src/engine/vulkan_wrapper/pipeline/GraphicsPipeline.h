//
// Created by leixing on 2024/12/28.
//

#pragma once

#include <vector>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanSwapchain.h"
#include "engine/vulkan_wrapper/VulkanRenderPass.h"
#include "engine/vulkan_wrapper/VulkanShader.h"
#include "engine/vulkan_wrapper/VulkanShaderModule.h"
#include "engine/vulkan_wrapper/pipeline/descriptor/DescriptorPool.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalVertexBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleVertexBuffer.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalIndexBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleIndexBuffer.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "engine/vulkan_wrapper/pipeline/descriptor/VulkanDescriptorBindingSets.h"
#include "engine/vulkan_wrapper/pipeline/resource/BufferInfo.h"
#include "engine/vulkan_wrapper/pipeline/resource/ImageInfo.h"
#include "engine/vulkan_wrapper/pipeline/resource/PipelineResource.h"
#include "engine/vulkan_wrapper/pipeline/descriptor/PipelineLayout.h"

namespace engine {

    class GraphicsPipeline {
    private:
        const VulkanDevice &mVulkanDevice;

        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        vk::Pipeline mPipeline;

        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> mVulkanVertexBuffers;
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        std::shared_ptr<VulkanDeviceLocalIndexBuffer> mIndexBuffer;

        // frame -> VulkanBufferDescriptorBindingSets
//        std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> mVulkanDescriptorBindingSets;

        // frame -> pipeline resource
        std::unique_ptr<PipelineLayout> mPipelineLayout;
        std::unique_ptr<DescriptorPool> mDescriptorPool;
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;
        std::vector<PipelineResource> mPipelineResources;

//        std::vector<vk::PushConstantRange> mPushConstantRanges;
//        std::vector<std::vector<uint8_t>> mPushConstantDataList;

    public:

        GraphicsPipeline(const VulkanDevice &vulkanDevice,
                         const VulkanSwapchain &swapchain,
                         const VulkanRenderPass &renderPass,
                         const VulkanShaderModule &vertexShaderModule,
                         const VulkanShaderModule &fragmentShaderModule,
                         const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                         const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                         std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> &&vertexBuffers,
                         std::shared_ptr<VulkanDeviceLocalIndexBuffer> indexBuffer,
                         std::unique_ptr<PipelineLayout>&& pipelineLayout,
                         uint32_t frameCount
                         /*std::unique_ptr<DescriptorPool> &&vulkanDescriptorPool,
                         const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                         std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> &&vulkanDescriptorBindingSets,
                         std::vector<vk::PushConstantRange> &&pushConstantRanges*/);

        ~GraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;

//        [[nodiscard]]
//        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;
//
//        [[nodiscard]]
//        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;
//
//        [[nodiscard]]
//        const std::vector<std::vector<uint8_t>> &getPushConstantDataList() const;

        GraphicsPipeline &createVertexBuffer(size_t size);

        GraphicsPipeline &createVertexBuffer(uint32_t binding, size_t size);

        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const void *data, size_t size);

        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const void *data, size_t size);

        template<typename T>
        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<T> &data) {
            return updateVertexBuffer(vulkanCommandPool, 0, data);
        }

        template<typename T>
        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const std::vector<T> &data) {
            if (index >= mVulkanVertexBuffers.size()) {
                LOG_E("index out of range, index:%d, size:%zu", index, mVulkanVertexBuffers.size());

                // Format the error message using std::to_string
                std::string errorMessage = "updateVertexBuffer: index out of range, index:" +
                                           std::to_string(index) +
                                           ", size:" +
                                           std::to_string(mVulkanVertexBuffers.size());
                throw std::runtime_error(errorMessage);
            }
            mVulkanVertexBuffers[index]->update(vulkanCommandPool, data.data(), data.size() * sizeof(T));

            return *this;
        }

        GraphicsPipeline &createIndexBuffer(size_t size);

        GraphicsPipeline &updateIndexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices);

//        GraphicsPipeline &updateUniformBuffer(const VulkanCommandPool &commandPool, uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

//        GraphicsPipeline &setDescriptorBindingBufferInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<BufferInfo> &&vulkanBufferInfo);
//
//        GraphicsPipeline &setDescriptorBindingImageInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<ImageInfo> &&vulkanImageInfo);

        GraphicsPipeline &updatePushConstant(uint32_t index, const void *data);

        void drawFrame(const vk::CommandBuffer &commandBuffer, uint32_t frameIndex);

    private:

        void updateDescriptorSets();
    };

} // engine
