//
// Created by leixing on 2024/12/16.
//

#include <cassert>
#include <utility>
#include <string>

#include "engine/VulkanEngine.h"
#include "engine/common/StringListSelector.h"
#include "engine/VkCheckCpp.h"
#include "engine/VkCheck.h"
#include "engine/Log.h"

namespace engine {

    VulkanEngine::VulkanEngine(std::unique_ptr<VulkanInstance> vulkanInstance,
                               std::unique_ptr<VulkanSurface> vulkanSurface,
                               std::unique_ptr<VulkanPhysicalDevice> vulkanPhysicalDevice,
                               std::unique_ptr<VulkanDevice> vulkanDevice,
                               std::unique_ptr<VulkanCommandPool> commandPool,
                               std::unique_ptr<VulkanSwapchain> swapchain,
                               std::unique_ptr<VulkanRenderPass> renderPass,
                               std::unique_ptr<VulkanGraphicsPipeline> graphicsPipeline,
                               std::unique_ptr<VulkanComputePipeline> computePipeline,
                               std::unique_ptr<VulkanFrameBuffer> frameBuffer,
                               std::unique_ptr<VulkanSyncObject> syncObject,
                               uint32_t frameCount) {

        mInstance = std::move(vulkanInstance);
        mSurface = std::move(vulkanSurface);
        mPhysicalDevice = std::move(vulkanPhysicalDevice);
        mVulkanDevice = std::move(vulkanDevice);
        mVulkanCommandPool = std::move(commandPool);
        mSwapchain = std::move(swapchain);
        mRenderPass = std::move(renderPass);
        mGraphicsPipeline = std::move(graphicsPipeline);
        mComputePipeline = std::move(computePipeline);
        mFrameBuffer = std::move(frameBuffer);
        mSyncObject = std::move(syncObject);
        mFrameCount = frameCount;

        LOG_D("VulkanEngine::init");
        if (mFrameCount <= 0) {
            throw std::runtime_error("mFrameCount <= 0");
        }
        if (mInstance == nullptr) {
            throw std::runtime_error("mInstance == nullptr");
        }
        if (mSurface == nullptr) {
            throw std::runtime_error("mSurface == nullptr");
        }
        if (mPhysicalDevice == nullptr) {
            throw std::runtime_error("mPhysicalDevice == nullptr");
        }
        if (mVulkanDevice == nullptr) {
            throw std::runtime_error("mDevice == nullptr");
        }
        if (mVulkanCommandPool == nullptr) {
            throw std::runtime_error("mCommandPool == nullptr");
        }
        if (mSwapchain == nullptr) {
            throw std::runtime_error("mSwapchain == nullptr");
        }
        if (mRenderPass == nullptr) {
            throw std::runtime_error("mRenderPass == nullptr");
        }
        if (mGraphicsPipeline == nullptr) {
            LOG_W("mGraphicsPipeline == nullptr");
        }
        if (mComputePipeline == nullptr) {
            LOG_W("mComputePipeline is null");
        }
        if (mFrameBuffer == nullptr) {
            throw std::runtime_error("mFrameBuffer == nullptr");
        }
        if (mSyncObject == nullptr) {
            throw std::runtime_error("mSyncObject == nullptr");
        }
    }

    VulkanEngine::~VulkanEngine() {
        LOG_D("~VulkanEngine()");

        mVulkanCommandPool.reset();

        mSyncObject.reset();

        mFrameBuffer.reset();

        mGraphicsPipeline.reset();
        mComputePipeline.reset();

        mRenderPass.reset();
        mSwapchain.reset();
        mVulkanDevice.reset();
        mSurface.reset();
        mInstance.reset();
    }

    vk::Instance VulkanEngine::getVKInstance() const {
        return mInstance->getInstance();
    }

    vk::Device VulkanEngine::getVKDevice() const {
        return mVulkanDevice->getDevice();
    }

    const VulkanCommandPool &VulkanEngine::getVulkanCommandPool() const {
        return *mVulkanCommandPool;
    }

    uint32_t VulkanEngine::getCurrentFrameIndex() const {
        return mCurrentFrameIndex;
    }

    VulkanGraphicsPipeline &VulkanEngine::getGraphicsPipeline() const {
        return *mGraphicsPipeline;
    }

    VulkanEngine &VulkanEngine::createVertexBuffer(size_t size) {
        getGraphicsPipeline().createVertexBuffer(0, size);
        return *this;
    }

    VulkanEngine &VulkanEngine::createVertexBuffer(uint32_t binding, size_t size) {
        getGraphicsPipeline().createVertexBuffer(binding, size);
        return *this;
    }

    VulkanEngine &VulkanEngine::createVertexBuffer(uint32_t pipelineIndex, uint32_t binding, size_t size) {
        if (pipelineIndex != 0) {
            throw std::runtime_error("other pipeline (index > 0) not supported yet !");
        }
        getGraphicsPipeline().createVertexBuffer(binding, size);
        return *this;
    }

    VulkanEngine &VulkanEngine::updateVertexBuffer(const void *data, size_t size) {
        return updateVertexBuffer(0, data, size);
    }

    VulkanEngine &VulkanEngine::updateVertexBuffer(uint32_t index, const void *data, size_t size) {
        getGraphicsPipeline().updateVertexBuffer(*mVulkanCommandPool, data, size);
        return *this;
    }

    VulkanEngine &VulkanEngine::createIndexBuffer(size_t size) {
        getGraphicsPipeline().createIndexBuffer(size);
        return *this;
    }

    VulkanEngine &VulkanEngine::updateIndexBuffer(const std::vector<uint32_t> &indices) {
        getGraphicsPipeline().updateIndexBuffer(*mVulkanCommandPool, indices);
        return *this;
    }

    VulkanEngine &VulkanEngine::updateUniformBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, void *data, uint32_t size) {
        getGraphicsPipeline().updateUniformBuffer(frameIndex, set, binding, data, size);
        return *this;
    }

    VulkanEngine &VulkanEngine::updatePushConstant(uint32_t index, const void *data) {
        getGraphicsPipeline().updatePushConstant(index, data);
        return *this;
    }

    void VulkanEngine::recreateSwapChain() {
        throw std::runtime_error("recreateSwapChain");
    }

    void VulkanEngine::drawFrame() {
        const vk::Device device = mVulkanDevice->getDevice();
        vk::Semaphore imageAvailableSemaphore = mSyncObject->getImageAvailableSemaphore(mCurrentFrameIndex);
        vk::Semaphore renderFinishedSemaphore = mSyncObject->getRenderFinishedSemaphore(mCurrentFrameIndex);
        vk::Fence fence = mSyncObject->getFence(mCurrentFrameIndex);

        vk::Result result = device.waitForFences(1, &fence, vk::True, std::numeric_limits<uint64_t>::max());
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = device.acquireNextImageKHR(mSwapchain->getSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore);
        if (acquireResult != vk::Result::eSuccess) {
            if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
                // 交换链已与表面不兼容，不能再用于渲染。通常在窗口大小调整后发生。
                LOG_E("acquireNextImageKHR: eErrorOutOfDateKHR, recreateSwapChain");
                recreateSwapChain();
                return;
            } else if (acquireResult == vk::Result::eSuboptimalKHR) {
                //vk::Result::eSuboptimalKHR 交换链仍然可以成功显示到表面，但表面属性不再完全匹配。
                LOG_D("acquireNextImageKHR: eSuboptimalKHR");
            } else {
                LOG_E("acquireNextImageKHR: failed: %d", acquireResult);
                throw std::runtime_error("acquireNextImageKHR failed");
            }
        }

        vk::CommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo
//                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
                .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse)
                .setPInheritanceInfo(nullptr);


        vk::CommandBuffer commandBuffer = mVulkanCommandPool->getCommandBuffers()[mCurrentFrameIndex];
        commandBuffer.reset();
        commandBuffer.begin(commandBufferBeginInfo);


        vk::Extent2D displaySize = mSwapchain->getDisplaySize();
        vk::Rect2D renderArea{};
        renderArea
                .setOffset(vk::Offset2D{0, 0})
                .setExtent(displaySize);

        vk::ClearValue colorClearValue = vk::ClearValue{mClearColor};
        vk::ClearValue depthStencilClearValue = vk::ClearValue{vk::ClearColorValue(mDepthStencil)};
        std::array<vk::ClearValue, 2> clearValues = {colorClearValue, depthStencilClearValue};

        vk::RenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo
                .setRenderPass(mRenderPass->getRenderPass())
                .setFramebuffer(mFrameBuffer->getFrameBuffers()[imageIndex])
                .setRenderArea(renderArea)
                .setClearValues(clearValues);
        /**
         * vk::SubpassContents::eInline
         * 子流程的渲染命令直接记录在当前的命令缓冲区中, 适用于简单的渲染流程，所有渲染命令都在同一个命令缓冲区中记录。
         *
         * vk::SubpassContents::eSecondaryCommandBuffers
         * 子流程的渲染命令通过次级命令缓冲区（Secondary Command Buffer）记录。
         * 适用于复杂的渲染流程，可以将渲染命令分散到多个次级命令缓冲区中，以提高代码的模块化和复用性。
         */
        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        mGraphicsPipeline->drawFrame(commandBuffer, mCurrentFrameIndex);

        commandBuffer.endRenderPass();
        commandBuffer.end();

        result = mSyncObject->resetFence(mCurrentFrameIndex);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        std::array<vk::Semaphore, 1> waitSemaphores = {imageAvailableSemaphore};
        std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        std::array<vk::CommandBuffer, 1> commandBuffers = {commandBuffer};
        std::array<vk::Semaphore, 1> signalSemaphores = {renderFinishedSemaphore};

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setWaitSemaphores(waitSemaphores)
                .setWaitDstStageMask(waitStages)
                .setCommandBuffers(commandBuffers)
                .setSignalSemaphores(signalSemaphores);

        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};
        mVulkanDevice->getGraphicsQueue().submit(submitInfos, fence);

        std::array<vk::SwapchainKHR, 1> swapChains = {mSwapchain->getSwapChain()};
        std::array<uint32_t, 1> imageIndices = {imageIndex};
        vk::PresentInfoKHR presentInfo{};
        presentInfo
                .setWaitSemaphores(signalSemaphores)
                .setSwapchains(swapChains)
                .setImageIndices(imageIndices);

//    std::cout << "presentKHR, mFrameBufferResized:" << mFrameBufferResized << std::endl;

        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
        try {
            result = mVulkanDevice->getPresentQueue().presentKHR(presentInfo);
        } catch (const vk::OutOfDateKHRError &e) {
            LOG_E("mPresentQueue.presentKHR => OutOfDateKHRError");
            result = vk::Result::eErrorOutOfDateKHR;
        }

        if (result != vk::Result::eSuccess) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFrameBufferResized) {
                mFrameBufferResized = false;
                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFrameBufferResized, recreateSwapChain");
                recreateSwapChain();
                return;
            } else {
                throw std::runtime_error("presentKHR failed");
            }
        }

        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
    }

} // engine
