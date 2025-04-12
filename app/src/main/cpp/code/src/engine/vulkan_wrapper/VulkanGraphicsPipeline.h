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
#include "engine/vulkan_wrapper/VulkanDescriptorPool.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalVertexBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleVertexBuffer.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalIndexBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleIndexBuffer.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBindingSets.h"
#include "engine/vulkan_wrapper/buffer/VulkanDescriptorBufferInfo.h"
#include "engine/vulkan_wrapper/image/VulkanDescriptorImageInfo.h"

namespace engine {

    class VulkanGraphicsPipeline {
    private:
        const VulkanDevice &mVulkanDevice;

        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        vk::Pipeline mPipeline;
        vk::PipelineLayout mPipelineLayout;

        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> mVulkanVertexBuffers;
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        std::shared_ptr<VulkanDeviceLocalIndexBuffer> mIndexBuffer;

        std::unique_ptr<VulkanDescriptorPool> mVulkanDescriptorPool;
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;

        // frame -> VulkanBufferDescriptorBindingSets
        std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> mVulkanDescriptorBindingSets;
//        std::vector<std::unordered_map<uint32_t, std::unordered_map<uint32_t, VulkanImageDescriptorBinding>>> mVulkanImageDescriptorBindings;

        std::vector<vk::PushConstantRange> mPushConstantRanges;
        std::vector<std::vector<uint8_t>> mPushConstantDataList;

    public:

        VulkanGraphicsPipeline(const VulkanDevice &vulkanDevice,
                               const VulkanSwapchain &swapchain,
                               const VulkanRenderPass &renderPass,
                               const VulkanShaderModule &vertexShaderModule,
                               const VulkanShaderModule &fragmentShaderModule,
                               const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                               const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                               std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> &&vertexBuffers,
                               std::shared_ptr<VulkanDeviceLocalIndexBuffer> indexBuffer,
                               uint32_t frameCount,
                               std::unique_ptr<VulkanDescriptorPool> &&vulkanDescriptorPool,
                               const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                               std::vector<std::unique_ptr<VulkanDescriptorBindingSets>>&& vulkanDescriptorBindingSets,
                               std::vector<vk::PushConstantRange> &&pushConstantRanges);

        ~VulkanGraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;

        [[nodiscard]]
        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;

        [[nodiscard]]
        const std::vector<std::vector<uint8_t>> &getPushConstantDataList() const;

        VulkanGraphicsPipeline &createVertexBuffer(size_t size);

        VulkanGraphicsPipeline &createVertexBuffer(uint32_t binding, size_t size);

        VulkanGraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const void *data, size_t size);

        VulkanGraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const void *data, size_t size);

        template<typename T>
        VulkanGraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<T> &data) {
            return updateVertexBuffer(vulkanCommandPool, 0, data);
        }

        template<typename T>
        VulkanGraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const std::vector<T> &data) {
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

        VulkanGraphicsPipeline &createIndexBuffer(size_t size);

        VulkanGraphicsPipeline &updateIndexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices);

        VulkanGraphicsPipeline &updateUniformBuffer(const VulkanCommandPool& commandPool, uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

        VulkanGraphicsPipeline &setDescriptorBindingBufferInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanBufferInfo);

        VulkanGraphicsPipeline &setDescriptorBindingImageInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<VulkanDescriptorImageInfo> &&vulkanImageInfo);

        VulkanGraphicsPipeline &updatePushConstant(uint32_t index, const void *data);

        void drawFrame(const vk::CommandBuffer &commandBuffer, uint32_t frameIndex);
    };

} // engine
