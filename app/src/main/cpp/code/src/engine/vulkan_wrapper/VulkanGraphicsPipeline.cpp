//
// Created by leixing on 2024/12/28.
//

#include "engine/vulkan_wrapper/VulkanGraphicsPipeline.h"
#include "engine/Log.h"

namespace engine {

    VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VulkanDevice &vulkanDevice,
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
                                                   std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> &&vulkanDescriptorBindingSets,
                                                   std::vector<vk::PushConstantRange> &&pushConstantRanges)
            : mVulkanDevice(vulkanDevice),
              mVulkanDescriptorPool(std::move(vulkanDescriptorPool)),
              mVulkanDescriptorBindingSets(std::move(vulkanDescriptorBindingSets)),
              mPushConstantRanges(std::move(pushConstantRanges)),
              mVulkanVertexBuffers(std::move(vertexBuffers)),
              mIndexBuffer(std::move(indexBuffer)) {

        vk::Device device = vulkanDevice.getDevice();

        // input assembler
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
        inputAssemblyStateCreateInfo
                .setTopology(vk::PrimitiveTopology::eTriangleList)
                .setPrimitiveRestartEnable(vk::False);

        std::vector<vk::DynamicState> dynamicStages = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        dynamicStateCreateInfo
                .setDynamicStates(dynamicStages);

        vk::Extent2D displaySize = swapchain.getDisplaySize();

        vk::Viewport viewport;
        viewport
                .setX(0.0f)
                .setY(0.0f)
                .setWidth((float) displaySize.width)
                .setHeight((float) displaySize.height)
                .setMinDepth(0.0f)
                .setMaxDepth(1.0f);
        mViewports.push_back(viewport);

        vk::Rect2D scissor{};
        scissor
                .setOffset(vk::Offset2D{0, 0})
                .setExtent(displaySize);
        mScissors.push_back(scissor);

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
        viewportStateCreateInfo
                .setViewports(mViewports)
                .setScissors(mScissors);

        // vertex shader
        vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
        vertexInputStateCreateInfo
                .setVertexBindingDescriptions(vertexInputBindingDescriptions)
                .setVertexAttributeDescriptions(vertexInputAttributeDescriptions);

        vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo;
        vertexShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eVertex)
                .setModule(vertexShaderModule.getShaderModule())
                .setPName("main")
                .setPSpecializationInfo(nullptr);

        // tessellation

        // geometry shader

        // rasterization
        vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
        rasterizationStateCreateInfo
                // 如果depthClampEnable设置为VK_TRUE，则超出近平面和远平面的片段将被夹紧，而不是丢弃它们。
                // 这在某些特殊情况下很有用，例如阴影贴图。使用此功能需要启用 GPU 功能。
                .setDepthClampEnable(vk::False)
                        // 如果rasterizerDiscardEnable设置为VK_TRUE，则几何图形永远不会通过光栅化阶段。这基本上禁用了帧缓冲区的任何输出。
                .setRasterizerDiscardEnable(vk::False)
                        //确定polygonMode如何为几何体生成片段。可以使用以下模式：
                        //VK_POLYGON_MODE_FILL：用碎片填充多边形区域
                        //VK_POLYGON_MODE_LINE：多边形边缘绘制为线
                        //VK_POLYGON_MODE_POINT：多边形顶点绘制为点
                .setPolygonMode(vk::PolygonMode::eFill)
                        // 使用填充以外的任何模式都需要设置 lineWidth :
                .setLineWidth(1.0f)
                        // 设置面剔除策略, 这里设置为反面被剔除
                .setCullMode(vk::CullModeFlagBits::eBack)
                        // 设置正面的方向
                .setFrontFace(vk::FrontFace::eCounterClockwise)
                .setDepthBiasEnable(vk::False)
                .setDepthBiasConstantFactor(0.0f)
                .setDepthBiasClamp(0.0f)
                .setDepthBiasSlopeFactor(0.0f);

        // depth & stencil testing
        vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
        depthStencilStateCreateInfo
                .setDepthTestEnable(vk::True)
                .setDepthWriteEnable(vk::True)
                .setDepthCompareOp(vk::CompareOp::eLess)
                .setDepthBoundsTestEnable(vk::False)
                .setMinDepthBounds(0.0f)
                .setMaxDepthBounds(1.0f)
                .setStencilTestEnable(vk::False)
                .setFront(vk::StencilOpState{})
                .setBack(vk::StencilOpState{});


        // Multisampling
        vk::PipelineMultisampleStateCreateInfo multiSampleStateCreateInfo;
        multiSampleStateCreateInfo
                .setSampleShadingEnable(vk::False)
                        // 光栅化时的采样数, 采样数越多，抗锯齿效果越好，但性能开销也越大
                        // vk::SampleCountFlagBits::e1：禁用多重采样（默认）。
                        // vk::SampleCountFlagBits::e2、e4、e8、e16：启用多重采样
                .setRasterizationSamples(vulkanDevice.getMsaaSamples())
                        // 最小采样着色率, 仅在 sampleShadingEnable 为 vk::True 时有效
                        // 0.0：禁用采样着色, 禁用（默认）。
                        // 1.0：对每个采样点执行完整的片段着色器, 最高质量。
                        // 0.2：对 20% 的采样点执行片段着色器
                .setMinSampleShading(0.2f)
                        // 采样掩码（Sample Mask），用于控制哪些采样点被启用,
                        // 每个采样点对应掩码中的一个位（bit），1 表示启用，0 表示禁用。
                .setPSampleMask(nullptr)
                        // 启用或禁用 Alpha 到覆盖（Alpha-to-Coverage）。
                        // 如果启用，片段的 Alpha 值将用于生成覆盖掩码，从而实现透明物体的抗锯齿
                .setAlphaToCoverageEnable(vk::False)
                        // 启用或禁用 Alpha 到一（Alpha-to-One）
                        // 如果启用，片段的 Alpha 值将被强制设置为 1.0
                .setAlphaToOneEnable(vk::False);

        // fragment shader
        vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo;
        fragmentShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eFragment)
                .setModule(fragmentShaderModule.getShaderModule())
                .setPName("main")
                .setPSpecializationInfo(nullptr);

        // color blending
        vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{};
        colorBlendAttachmentState
//                .setBlendEnable(vk::True)
                .setBlendEnable(vk::False)
                .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                                   | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
                .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
                .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
                .setColorBlendOp(vk::BlendOp::eAdd)
                .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
                .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
                .setAlphaBlendOp(vk::BlendOp::eAdd);
        std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachmentStates = {colorBlendAttachmentState};

        vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
        colorBlendStateCreateInfo
                .setLogicOpEnable(vk::False)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachments(colorBlendAttachmentStates)
                .setBlendConstants(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo
                .setSetLayouts(descriptorSetLayouts)
                .setPushConstantRanges(mPushConstantRanges);

        mPipelineLayout = vulkanDevice.getDevice().createPipelineLayout(pipelineLayoutCreateInfo);

        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageCreateInfos = {vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};

        vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
        graphicsPipelineCreateInfo
                .setStages(shaderStageCreateInfos)
                .setPVertexInputState(&vertexInputStateCreateInfo)
                .setPInputAssemblyState(&inputAssemblyStateCreateInfo)
                .setPViewportState(&viewportStateCreateInfo)
                .setPRasterizationState(&rasterizationStateCreateInfo)
                .setPMultisampleState(&multiSampleStateCreateInfo)
                .setPDepthStencilState(&depthStencilStateCreateInfo)
                .setPColorBlendState(&colorBlendStateCreateInfo)
                .setPDynamicState(&dynamicStateCreateInfo)
                .setLayout(mPipelineLayout)
                .setRenderPass(renderPass.getRenderPass())
                .setSubpass(0)
                .setBasePipelineHandle(nullptr)
                .setBasePipelineIndex(-1);

        auto [result, pipeline] = device.createGraphicsPipeline(nullptr, graphicsPipelineCreateInfo);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("createGraphicsPipelines failed");
        }
        mPipeline = pipeline;

        // vertex buffers
        for (const std::shared_ptr<VulkanDeviceLocalVertexBuffer> &vulkanVertexBuffer: mVulkanVertexBuffers) {
            if (vulkanVertexBuffer != nullptr) {
                mVertexBuffers.push_back(vulkanVertexBuffer->getBuffer());
            } else {
                mVertexBuffers.push_back(nullptr);
            }
            mVertexBufferOffsets.push_back(0);
        }

        // descriptors
        for (int i = 0; i < frameCount; i++) {
            mDescriptorSets.push_back(mVulkanDescriptorPool->allocateDescriptorSets(descriptorSetLayouts));
        }

        // updateDescriptorSets
        std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

        for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
            const std::unique_ptr<VulkanDescriptorBindingSets> &vulkanDescriptorBindingSetOfFrame = mVulkanDescriptorBindingSets[frameIndex];
            if (vulkanDescriptorBindingSetOfFrame == nullptr) {
                continue;
            }
            for (const auto &setsEntry: vulkanDescriptorBindingSetOfFrame->getVulkanDescriptorBindingSets()) {
                uint32_t set = setsEntry.first;
                const std::unique_ptr<VulkanDescriptorBindingSet> &vulkanDescriptorBindingSet = setsEntry.second;

                for (const auto &bindingEntry: vulkanDescriptorBindingSet->getVulkanDescriptorBindings()) {
                    uint32_t binding = bindingEntry.first;
                    const std::unique_ptr<VulkanDescriptorBinding> &vulkanDescriptorBinding = bindingEntry.second;
                    vk::DescriptorType type = vulkanDescriptorBinding->getDescriptorType();
                    if (type == vk::DescriptorType::eUniformBuffer) {
                        const std::unique_ptr<VulkanDescriptorBufferInfo> &bufferInfo = vulkanDescriptorBinding->getVulkanDescriptorBufferInfo();
                        if (bufferInfo != nullptr) {
                            vk::WriteDescriptorSet writeDescriptorSet{};

                            vk::DescriptorBufferInfo descriptorBufferInfo = bufferInfo->createDescriptorBufferInfo();
                            std::array<vk::DescriptorBufferInfo, 1> descriptorBufferInfos = {descriptorBufferInfo};

                            writeDescriptorSet
                                    .setDstSet(mDescriptorSets[frameIndex][set])
                                    .setDstBinding(binding)
                                    .setDstArrayElement(vulkanDescriptorBinding->getDescriptorOffset())
                                    .setDescriptorCount(vulkanDescriptorBinding->getDescriptorRange())
                                    .setDescriptorType(vulkanDescriptorBinding->getDescriptorType())
                                    .setBufferInfo(descriptorBufferInfos);
                            writeDescriptorSets.push_back(writeDescriptorSet);
                        }
                    } else if (type == vk::DescriptorType::eCombinedImageSampler) {
                        const std::unique_ptr<VulkanDescriptorImageInfo> &imageInfo = vulkanDescriptorBinding->getVulkanDescriptorImageInfo();
                        if (imageInfo != nullptr) {
                            vk::WriteDescriptorSet writeDescriptorSet{};

                            vk::DescriptorImageInfo descriptorImageInfo = imageInfo->createDescriptorImageInfo();
                            std::array<vk::DescriptorImageInfo, 1> descriptorImageInfos = {descriptorImageInfo};

                            writeDescriptorSet
                                    .setDstSet(mDescriptorSets[frameIndex][set])
                                    .setDstBinding(binding)
                                    .setDstArrayElement(vulkanDescriptorBinding->getDescriptorOffset())
                                    .setDescriptorCount(vulkanDescriptorBinding->getDescriptorRange())
                                    .setDescriptorType(vulkanDescriptorBinding->getDescriptorType())
                                    .setImageInfo(descriptorImageInfos);
                            writeDescriptorSets.push_back(writeDescriptorSet);
                        }
                    } else {
                        throw std::runtime_error("unknown descriptor type");
                    }

                }
            }
        }

        if (!writeDescriptorSets.empty()) {
            vulkanDevice.getDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
        }

        for (const vk::PushConstantRange &pushConstantRange: mPushConstantRanges) {
            // 创建数据缓冲区
            std::vector<uint8_t> data(pushConstantRange.size);
            mPushConstantDataList.push_back(std::move(data));
        }
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
        LOG_D("VulkanGraphicsPipeline::~VulkanGraphicsPipeline");
        vk::Device device = mVulkanDevice.getDevice();
        device.destroy(mPipeline);
        device.destroy(mPipelineLayout);

        mIndexBuffer.reset();
        mVertexBuffers.clear();
    }

    const vk::Pipeline &VulkanGraphicsPipeline::getPipeline() const {
        return mPipeline;
    }

    const vk::PipelineLayout &VulkanGraphicsPipeline::getPipelineLayout() const {
        return mPipelineLayout;
    }

    const std::vector<vk::DescriptorSet> &VulkanGraphicsPipeline::getDescriptorSets(uint32_t frameIndex) const {
        return mDescriptorSets[frameIndex];
    }

    const std::vector<vk::PushConstantRange> &VulkanGraphicsPipeline::getPushConstantRanges() const {
        return mPushConstantRanges;
    }

    const std::vector<std::vector<uint8_t>> &VulkanGraphicsPipeline::getPushConstantDataList() const {
        return mPushConstantDataList;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::createVertexBuffer(size_t size) {
        return createVertexBuffer(0, size);
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::createVertexBuffer(uint32_t binding, size_t size) {
        mVulkanVertexBuffers[binding] = std::make_shared<VulkanDeviceLocalVertexBuffer>(mVulkanDevice, size);
        mVertexBuffers[binding] = mVulkanVertexBuffers.back()->getBuffer();
        mVertexBufferOffsets[binding] = 0;

        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, const void *data, size_t size) {
        return updateVertexBuffer(vulkanCommandPool, 0, data, size);
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::updateVertexBuffer(const VulkanCommandPool &vulkanCommandPool, uint32_t index, const void *data, size_t size) {
        if (index >= mVulkanVertexBuffers.size()) {
            LOG_E("index out of range, index:%d, size:%zu", index, mVulkanVertexBuffers.size());

            // Format the error message using std::to_string
            std::string errorMessage = "updateVertexBuffer: index out of range, index:" +
                                       std::to_string(index) +
                                       ", size:" +
                                       std::to_string(mVulkanVertexBuffers.size());
            throw std::runtime_error(errorMessage);
        }
        mVulkanVertexBuffers[index]->update(vulkanCommandPool, data, size);

        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::createIndexBuffer(size_t size) {
        mIndexBuffer.reset();
        mIndexBuffer = std::make_unique<VulkanDeviceLocalIndexBuffer>(mVulkanDevice, size);

        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::updateIndexBuffer(const VulkanCommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices) {
        mIndexBuffer->update(vulkanCommandPool, indices);
        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::updateUniformBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, void *data, uint32_t size) {

        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::setDescriptorBindingBufferInfo(uint32_t frameIndex, uint32_t set, uint32_t binding,
                                                                                   std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanBufferInfo) {
        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::setDescriptorBindingImageInfo(uint32_t frameIndex, uint32_t set, uint32_t binding,
                                                                                  std::unique_ptr<VulkanDescriptorImageInfo> &&vulkanImageInfo) {
        return *this;
    }

    VulkanGraphicsPipeline &VulkanGraphicsPipeline::updatePushConstant(uint32_t index, const void *data) {
        std::memcpy(mPushConstantDataList[index].data(), data, mPushConstantRanges[index].size);
        return *this;
    }

    void VulkanGraphicsPipeline::drawFrame(const vk::CommandBuffer &commandBuffer, uint32_t frameIndex) {

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);

        /**
         * firstViewport 在某些情况下，可能需要将视口绑定到特定的范围，而不是从索引 0 开始
         * 类似于 copy中的 dst_Index [s,s,s] -> [_,_,_, d,d,d, _,_,...] (firstViewport=3)
         */
        commandBuffer.setViewport(0, mViewports);
        commandBuffer.setScissor(0, mScissors);

        /**
         * offsets 参数仅指定顶点数据的起始位置，
         * 读取的数据量由绘制命令（如 vkCmdDraw 或 vkCmdDrawIndexed）的顶点数量决定，结合顶点输入绑定描述中的 stride 参数隐式计算总字节数
         * 总字节数 = vertexCount × stride
         * 示例:
         * 起始位置：offsets[0]（如 256 字节）
         * 结束位置：offsets[0] + vertexCount × stride（如 256 + 3×20 = 316 字节）
         */
        commandBuffer.bindVertexBuffers(0, mVertexBuffers, mVertexBufferOffsets);
        commandBuffer.bindIndexBuffer(mIndexBuffer->getBuffer(), 0, vk::IndexType::eUint32);
        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout, 0, mDescriptorSets[frameIndex], nullptr);

        // push constants
        if (!mPushConstantRanges.empty()) {
            for (uint32_t pushConstantIndex = 0; pushConstantIndex < mPushConstantRanges.size(); pushConstantIndex++) {
                const vk::PushConstantRange &pushConstantRange = mPushConstantRanges[pushConstantIndex];
                const std::vector<uint8_t> pushConstantData = mPushConstantDataList[pushConstantIndex];

                commandBuffer.pushConstants(mPipelineLayout, pushConstantRange.stageFlags,
                                            pushConstantRange.offset,
                                            pushConstantRange.size,
                                            pushConstantData.data());
            }
        }

        // draw call
        /**
         * uint32_t           indexCount,
         * uint32_t           instanceCount,
         * uint32_t           firstIndex,
         * int32_t            vertexOffset,
         * uint32_t           firstInstance,
         */
        commandBuffer.drawIndexed(mIndexBuffer->getIndicesCount(), 1, 0, 0, 0);

    }

} // engine