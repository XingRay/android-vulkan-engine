//
// Created by leixing on 2024/12/16.
//

#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanSurface.h"
#include "engine/vulkan_wrapper/VulkanPhysicalDevice.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanShader.h"
#include "engine/vulkan_wrapper/VulkanSwapchain.h"
#include "engine/vulkan_wrapper/VulkanRenderPass.h"
#include "engine/vulkan_wrapper/pipeline/GraphicsPipeline.h"
#include "engine/vulkan_wrapper/pipeline/ComputePipeline.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalVertexBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleVertexBuffer.h"
#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalIndexBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanHostVisibleIndexBuffer.h"
#include "engine/vulkan_wrapper//buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "engine/vulkan_wrapper/VulkanSyncObject.h"
#include "engine/vulkan_wrapper/VulkanFrameBuffer.h"
#include "engine/vulkan_wrapper/sampler/DefaultSampler.h"

#include "engine/common/StringListSelector.h"
#include "engine/configure/physical_device/VulkanPhysicalDeviceProvider.h"

namespace engine {

    class VulkanEngine {
    private:
        bool mFrameBufferResized = false;
        uint32_t mFrameCount = 2;
        uint32_t mCurrentFrameIndex = 0;
//        const std::array<float, 4> mClearColor = {0.05f, 0.05f, 0.05f, 1.0f};
        const std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.6f, 1.0f};
        const std::array<float, 4> mDepthStencil = {1.0f, 0, 0, 0};

        std::unique_ptr<VulkanInstance> mInstance;
        std::unique_ptr<VulkanSurface> mSurface;
        std::unique_ptr<VulkanPhysicalDevice> mPhysicalDevice;
        std::unique_ptr<VulkanDevice> mVulkanDevice;

        std::unique_ptr<VulkanSwapchain> mSwapchain;
        std::unique_ptr<VulkanRenderPass> mRenderPass;

        // todo std::vector<VulkanGraphicsPipeline> mGraphicsPipelines;
        std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
        std::unique_ptr<ComputePipeline> mComputePipeline;

        std::unique_ptr<VulkanCommandPool> mVulkanCommandPool;
        std::unique_ptr<VulkanFrameBuffer> mFrameBuffer;

        std::unique_ptr<VulkanSyncObject> mSyncObject;

    public:
        VulkanEngine(std::unique_ptr<VulkanInstance> vulkanInstance,
                     std::unique_ptr<VulkanSurface> vulkanSurface,
                     std::unique_ptr<VulkanPhysicalDevice> vulkanPhysicalDevice,
                     std::unique_ptr<VulkanDevice> vulkanDevice,
                     std::unique_ptr<VulkanCommandPool> commandPool,
                     std::unique_ptr<VulkanSwapchain> swapchain,
                     std::unique_ptr<VulkanRenderPass> renderPass,
                     std::unique_ptr<GraphicsPipeline> graphicsPipeline,
                     std::unique_ptr<ComputePipeline> computePipeline,
                     std::unique_ptr<VulkanFrameBuffer> frameBuffer,
                     std::unique_ptr<VulkanSyncObject> syncObject,
                     uint32_t frameCount);

        ~VulkanEngine();

        [[nodiscard]]
        vk::Instance getVKInstance() const;

        [[nodiscard]]
        vk::Device getVKDevice() const;

        [[nodiscard]]
        const VulkanCommandPool &getVulkanCommandPool() const;

        [[nodiscard]]
        uint32_t getCurrentFrameIndex() const;

        [[nodiscard]]
        GraphicsPipeline &getGraphicsPipeline() const;

        VulkanEngine &createVertexBuffer(size_t size);

        VulkanEngine &createVertexBuffer(uint32_t binding, size_t size);

        VulkanEngine &createVertexBuffer(uint32_t pipelineIndex, uint32_t binding, size_t size);

        VulkanEngine &updateVertexBuffer(const void *data, size_t size);

        VulkanEngine &updateVertexBuffer(uint32_t index, const void *data, size_t size);

        template<typename T>
        VulkanEngine &updateVertexBuffer(const std::vector<T> &data) {
            return updateVertexBuffer(0, data);
        }

        template<typename T>
        VulkanEngine &updateVertexBuffer(uint32_t index, const std::vector<T> &data) {
//            getGraphicsPipeline().updateVertexBuffer(*mVulkanCommandPool, data.data(), data.size() * sizeof(T));
            return *this;
        }

        VulkanEngine &createIndexBuffer(size_t size);

        VulkanEngine &updateIndexBuffer(const std::vector<uint32_t> &indices);

        VulkanEngine &updateUniformBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

        template<class T>
        VulkanEngine &updateUniformBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, const T &data) {
//            getGraphicsPipeline().updateUniformBuffer(*mVulkanCommandPool, frameIndex, set, binding, static_cast<const void *>(&data), sizeof(T));
            return *this;
        }

        VulkanEngine &updateCurrentFrameUniformBuffer(uint32_t set, uint32_t binding, const void *data, uint32_t size);

        template<class T>
        VulkanEngine &updateCurrentFrameUniformBuffer(uint32_t set, uint32_t binding, const T &data) {
            return updateCurrentFrameUniformBuffer(set, binding, static_cast<const void *>(&data), sizeof(T));
        }

        VulkanEngine &updatePushConstant(uint32_t index, const void *data);

        void drawFrame();

    private:
        void recreateSwapChain();
    };


} // engine
