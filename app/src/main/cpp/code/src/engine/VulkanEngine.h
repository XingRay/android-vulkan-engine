//
// Created by leixing on 2024/12/16.
//

#pragma once

#include <vector>

#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptorSet.h"
#include "VulkanPipeline.h"
#include "VulkanCommandPool.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanSyncObject.h"
#include "VulkanFrameBuffer.h"
#include "VulkanVertexShader.h"
#include "VulkanFragmentShader.h"

namespace engine {

    class VulkanEngine {
    public:
        bool mFrameBufferResized = false;
        const int mFrameCount = 2;
        uint32_t mCurrentFrame = 0;

        const std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.6f, 1.0f};
        const std::array<float, 4> mDepthStencil = {1.0f, 0, 0, 0};

        std::unique_ptr<VulkanInstance> mInstance;
        std::unique_ptr<VulkanSurface> mSurface;
        std::unique_ptr<VulkanDevice> mDevice;
        std::unique_ptr<VulkanSwapchain> mSwapchain;
        std::unique_ptr<VulkanRenderPass> mRenderPass;

        std::unique_ptr<VulkanDescriptorSet> mDescriptorSet;
        std::unique_ptr<VulkanPipeline> mPipeline;
        std::unique_ptr<VulkanCommandPool> mCommandPool;
        std::unique_ptr<VulkanFrameBuffer> mFrameBuffer;

        std::vector<std::unique_ptr<VulkanVertexBuffer>> mVulkanVertexBuffers;
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        std::unique_ptr<VulkanIndexBuffer> mIndexBuffer;

        std::vector<VulkanUniformBuffer> mTransformUniformBuffers;
        std::vector<VulkanUniformBuffer> mColorUniformBuffers;

        std::unique_ptr<VulkanSyncObject> mSyncObject;

    public:
        explicit VulkanEngine(const std::vector<const char *> &instanceExtensions,
                              const std::vector<const char *> &layers,
                              int frameCount = 2);

        ~VulkanEngine();

        [[nodiscard]]
        vk::Instance getVKInstance() const;

        [[nodiscard]]
        vk::Device getVKDevice() const;

        // Initialize vulkan device context
        // after return, vulkan is ready to draw
        bool initVulkan(std::unique_ptr<VulkanSurface> &vulkanSurface,
                        const std::vector<const char *> &deviceExtensions,
                        const std::unique_ptr<VulkanVertexShader> &vertexShader,
                        const std::unique_ptr<VulkanFragmentShader> &fragmentShader);

        void drawFrame();

        void createDirectlyTransferVertexBuffer(size_t size);

        void createStagingTransferVertexBuffer(size_t size);

        void updateVertexBuffer(const void *data, size_t size);

        void updateVertexBuffer(uint32_t index, const void *data, size_t size);

        template<typename T>
        void updateVertexBuffer(const std::vector<T> &data) {
            updateVertexBuffer(0, data);
        }

        template<typename T>
        void updateVertexBuffer(uint32_t index, const std::vector<T> &data) {
            if (index >= mVulkanVertexBuffers.size()) {
                LOG_E("index out of range, index:%d, size:%zu", index, mVulkanVertexBuffers.size());

                // Format the error message using std::to_string
                std::string errorMessage = "updateVertexBuffer: index out of range, index:" +
                                           std::to_string(index) +
                                           ", size:" +
                                           std::to_string(mVulkanVertexBuffers.size());
                throw std::runtime_error(errorMessage);
            }
            mVulkanVertexBuffers[index]->update(data);
        }

        void createDirectlyTransferIndexBuffer(size_t size);

        void createStagingTransferIndexBuffer(size_t size);

        void updateIndexBuffer(std::vector<uint32_t> indices) const;

    private:
        void recreateSwapChain();
    };

} // engine
