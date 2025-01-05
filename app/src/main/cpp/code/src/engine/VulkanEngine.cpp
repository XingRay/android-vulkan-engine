//
// Created by leixing on 2024/12/16.
//

#include "VulkanEngine.h"
#include "VkCheckCpp.h"
#include "VkCheck.h"
#include "Log.h"
#include <cassert>
#include <utility>


namespace engine {

    VulkanEngine::VulkanEngine(const std::vector<const char *> &instanceExtensions, const std::vector<const char *> &layers, int frameCount)
            : mFrameCount(frameCount) {
        mInstance = std::make_unique<VulkanInstance>(instanceExtensions, layers);
    }

    VulkanEngine::~VulkanEngine() {
        LOG_D("~VulkanEngine()");

        mCommandPool.reset();

        mSyncObject.reset();

        mColorUniformBuffers.clear();
        mTransformUniformBuffers.clear();

        mIndexBuffer.reset();
        mVertexBuffers.clear();
        mFrameBuffer.reset();

        mPipeline.reset();
        mDescriptorSet.reset();

        mRenderPass.reset();
        mSwapchain.reset();
        mDevice.reset();
        mSurface.reset();
        mInstance.reset();
    }

    vk::Instance VulkanEngine::getVKInstance() const {
        return mInstance->getInstance();
    }

    vk::Device VulkanEngine::getVKDevice() const {
        return mDevice->getDevice();
    }

    bool VulkanEngine::initVulkan(std::unique_ptr<VulkanSurface> &vulkanSurface,
                                  const std::vector<const char *> &deviceExtensions,
                                  const std::unique_ptr<VulkanVertexShader> &vertexShader,
                                  const std::vector<char> &fragmentShaderCode) {

        mSurface = std::move(vulkanSurface);
        mDevice = std::make_unique<VulkanDevice>(mInstance->getInstance(), mSurface->getSurface(), mInstance->getEnabledLayers(), deviceExtensions);
        vk::Device device = mDevice->getDevice();

        vk::Extent2D currentExtent = mDevice->getCapabilities().currentExtent;
        LOG_D("currentExtent:[%d x %d]", currentExtent.width, currentExtent.height);
        mSwapchain = std::make_unique<VulkanSwapchain>(*mDevice, *mSurface, currentExtent.width, currentExtent.height);

        mRenderPass = std::make_unique<VulkanRenderPass>(*mDevice, *mSwapchain);

        mDescriptorSet = std::make_unique<VulkanDescriptorSet>(*mDevice, mFrameCount
                /*transformUniformBuffers, sizeof(app::TransformUniformBufferObject),
                colorUniformBuffers, sizeof(app::ColorUniformBufferObject)*/);

        vk::ShaderModule vertexModule = mDevice->createShaderModule(vertexShader->getShaderCode());
        vk::ShaderModule fragmentModule = mDevice->createShaderModule(fragmentShaderCode);

        mPipeline = std::make_unique<VulkanPipeline>(*mDevice, *mSwapchain, *mDescriptorSet, *mRenderPass,
                                                     vertexModule, fragmentModule,
                                                     vertexShader->getVertexDescriptions(),
                                                     vertexShader->getVertexInputAttributeDescriptions());
        device.destroy(vertexModule);
        device.destroy(fragmentModule);

        mCommandPool = std::make_unique<VulkanCommandPool>(*mDevice, mFrameCount);
        mFrameBuffer = std::make_unique<VulkanFrameBuffer>(*mDevice, *mSwapchain, *mRenderPass, *mCommandPool);

//        uint32_t vertexSize = sizeof(mVertices[0]) * mVertices.size();
//        mVertexBuffer = std::make_unique<VulkanVertexBuffer>(*mDevice, vertexSize);
//        mVertexBuffer->updateByStageBuffer(*mCommandPool, mVertices.data(), vertexSize);
//        mVertexBuffer->update(&mVertices, vertexSize);

//        uint32_t indicesSize = sizeof(uint32_t) * mIndices.size();
//        mIndexBuffer = std::make_unique<VulkanIndexBuffer>(*mDevice, indicesSize);
//        mIndexBuffer->updateByStageBuffer(*mCommandPool, mIndices.data(), indicesSize);
//        mIndexBuffer->update(&mIndices, indicesSize);

        mSyncObject = std::make_unique<VulkanSyncObject>(*mDevice, mFrameCount);

        return true;
    }


    void VulkanEngine::drawFrame() {
//        LOG_D("VulkanEngine::VulkanDrawFrame(), mCurrentFrame:%d", mCurrentFrame);
        const vk::Device device = mDevice->getDevice();

        vk::Result result = mSyncObject->waitFence(mCurrentFrame);
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        auto [acquireResult, imageIndex] = device.acquireNextImageKHR(mSwapchain->getSwapChain(), std::numeric_limits<uint64_t>::max(), mSyncObject->getImageAvailableSemaphore(mCurrentFrame));
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

        // 检查当前帧是否已经在使用
        vk::CommandBuffer currentFrameCommandBuffer = mCommandPool->getCommandBuffers()[mCurrentFrame];
        currentFrameCommandBuffer.reset();
        mCommandPool->recordCommandInRenderPass(currentFrameCommandBuffer,
                                                mFrameBuffer->getFrameBuffers()[imageIndex],
                                                mRenderPass->getRenderPass(),
                                                mPipeline->getPipeline(),
                                                mSwapchain->getDisplaySize(),
                                                mClearColor,
                                                mDepthStencil,
                                                [&](vk::CommandBuffer commandBuffer) -> void {
                                                    commandBuffer.bindVertexBuffers(0, mVertexBuffers, mVertexBufferOffsets);
                                                    commandBuffer.bindIndexBuffer(mIndexBuffer->getIndexBuffer(), 0, vk::IndexType::eUint32);
                                                    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipeline->getPipelineLayout(), 0,
                                                                                     {mDescriptorSet->getDescriptorSets()[mCurrentFrame]}, nullptr);
                                                    commandBuffer.drawIndexed(mIndexBuffer->getIndicesCount(), 1, 0, 0, 0);
                                                });

        result = mSyncObject->resetFence(mCurrentFrame);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        std::array<vk::Semaphore, 1> waitSemaphores = {mSyncObject->getImageAvailableSemaphore(mCurrentFrame)};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        std::array<vk::Semaphore, 1> signalSemaphores = {mSyncObject->getRenderFinishedSemaphore(mCurrentFrame)};
        std::array<vk::CommandBuffer, 1> commandBuffers = {currentFrameCommandBuffer};

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setWaitSemaphores(waitSemaphores)
                .setPWaitDstStageMask(waitStages)
                .setCommandBuffers(commandBuffers)
                .setSignalSemaphores(signalSemaphores);

        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};
        mDevice->getGraphicsQueue().submit(submitInfos, mSyncObject->getFence(mCurrentFrame));

        std::array<vk::SwapchainKHR, 1> swapchains = {mSwapchain->getSwapChain()};
        vk::PresentInfoKHR presentInfo{};
        presentInfo.setWaitSemaphores(signalSemaphores)
                .setSwapchains(swapchains)
                .setImageIndices(imageIndex);

//    std::cout << "presentKHR, mFrameBufferResized:" << mFrameBufferResized << std::endl;

        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
        try {
            result = mDevice->getPresentQueue().presentKHR(presentInfo);
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

        mCurrentFrame = (mCurrentFrame + 1) % mFrameCount;
    }

    void VulkanEngine::createDirectlyTransferVertexBuffer(size_t size) {
        std::unique_ptr<DirectlyTransferVertexBuffer> vertexBuffer = std::make_unique<DirectlyTransferVertexBuffer>(*mDevice, size);
        mVulkanVertexBuffers.push_back(std::move(vertexBuffer));
        mVertexBuffers.push_back(mVulkanVertexBuffers.back()->getVertexBuffer());
        mVertexBufferOffsets.push_back(0);
    }

    void VulkanEngine::createStagingTransferVertexBuffer(size_t size) {
        std::unique_ptr<StagingTransferVertexBuffer> vertexBuffer = std::make_unique<StagingTransferVertexBuffer>(*mDevice, *mCommandPool, size);
        mVulkanVertexBuffers.push_back(std::move(vertexBuffer));
        mVertexBuffers.push_back(mVulkanVertexBuffers.back()->getVertexBuffer());
        mVertexBufferOffsets.push_back(0);
    }

    void VulkanEngine::updateVertexBuffer(const void *data, size_t size) {
        mVulkanVertexBuffers[0]->update(data, size);
    }

    template<typename T>
    void VulkanEngine::updateVertexBuffer(const std::vector<T> &data) {
        mVulkanVertexBuffers[0]->update(data);
    }

    void VulkanEngine::createDirectlyTransferIndexBuffer(size_t size) {
        mIndexBuffer.reset();
        mIndexBuffer = std::make_unique<DirectlyTransferIndexBuffer>(*mDevice, size);
    }

    void VulkanEngine::createStagingTransferIndexBuffer(size_t size) {
        mIndexBuffer.reset();
        mIndexBuffer = std::make_unique<StagingTransferIndexBuffer>(*mDevice, *mCommandPool, size);
    }

    void VulkanEngine::updateIndexBuffer(std::vector<uint32_t> indices) const {
        mIndexBuffer->update(std::move(indices));
    }

    void VulkanEngine::recreateSwapChain() {
        throw std::runtime_error("recreateSwapChain");
    }

} // engine
