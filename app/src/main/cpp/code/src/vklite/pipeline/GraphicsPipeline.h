//
// Created by leixing on 2024/12/28.
//

#pragma once

#include <vector>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/swapchain/VulkanSwapchain.h"
#include "vklite/VulkanRenderPass.h"
#include "vklite/shader/VulkanShader.h"
#include "vklite/shader/VulkanShaderModule.h"
#include "vklite/pipeline/descriptor/DescriptorPool.h"
#include "vklite/command/VulkanCommandPool.h"
#include "vklite/buffer/device_local/VulkanDeviceLocalIndexBuffer.h"
#include "vklite/buffer/host_visible/VulkanHostVisibleIndexBuffer.h"
#include "vklite/buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "vklite/pipeline/descriptor/VulkanDescriptorBindingSets.h"
#include "vklite/pipeline_resource/BufferInfo.h"
#include "vklite/pipeline_resource/ImageInfo.h"
#include "vklite/pipeline/descriptor/PipelineLayout.h"
#include "vklite/vertex/description/VertexBufferDescription.h"
#include "vklite/pipeline_resource/PipelineResource.h"

namespace vklite {

    class GraphicsPipeline {
    private:
        const VulkanDevice &mVulkanDevice;

        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        vk::Pipeline mPipeline;

        std::unique_ptr<PipelineLayout> mPipelineLayout;

        // frame -> pipeline resource
//        std::vector<PipelineResource> mPipelineResources;

    public:

        GraphicsPipeline(const VulkanDevice &vulkanDevice,
                         const VulkanSwapchain &swapchain,
                         const VulkanRenderPass &renderPass,
                         const VulkanShaderModule &vertexShaderModule,
                         const VulkanShaderModule &fragmentShaderModule,
                         const VertexBufferDescription &vertexBufferDescription,
                         std::unique_ptr<PipelineLayout> &&pipelineLayout/*,
                         std::vector<PipelineResource>&& pipelineResources*/);

        ~GraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

//        [[nodiscard]]
//        const vk::PipelineLayout &getPipelineLayout() const;

//        [[nodiscard]]
//        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;
//
//        [[nodiscard]]
//        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;
//
//        [[nodiscard]]
//        const std::vector<std::vector<uint8_t>> &getPushConstantDataList() const;

//        GraphicsPipeline &createVertexBuffer(size_t size);
//
//        GraphicsPipeline &createVertexBuffer(uint32_t binding, size_t size);
//
//        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const void *data, size_t size);
//
//        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const void *data, size_t size);
//
//        template<typename T>
//        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<T> &data) {
//            return updateVertexBuffer(vulkanCommandPool, 0, data);
//        }

//        template<typename T>
//        GraphicsPipeline &updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const std::vector<T> &data) {
//            if (index >= mVulkanVertexBuffers.size()) {
//                LOG_E("index out of range, index:%d, size:%zu", index, mVulkanVertexBuffers.size());
//
//                // Format the error message using std::to_string
//                std::string errorMessage = "updateVertexBuffer: index out of range, index:" +
//                                           std::to_string(index) +
//                                           ", size:" +
//                                           std::to_string(mVulkanVertexBuffers.size());
//                throw std::runtime_error(errorMessage);
//            }
//            mVulkanVertexBuffers[index]->update(vulkanCommandPool, data.data(), data.size() * sizeof(T));
//
//            return *this;
//        }

//        GraphicsPipeline &createIndexBuffer(size_t size);
//
//        GraphicsPipeline &updateIndexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices);

//        GraphicsPipeline &updateUniformBuffer(const VulkanCommandPool &commandPool, uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

//        GraphicsPipeline &setDescriptorBindingBufferInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<BufferInfo> &&vulkanBufferInfo);
//
//        GraphicsPipeline &setDescriptorBindingImageInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<ImageInfo> &&vulkanImageInfo);

//        GraphicsPipeline &updatePushConstant(uint32_t index, const void *data);

        void drawFrame(const vk::CommandBuffer &commandBuffer, const PipelineResource &pipelineResource);

    private:

//        void updateDescriptorSets();
    };

} // engine
