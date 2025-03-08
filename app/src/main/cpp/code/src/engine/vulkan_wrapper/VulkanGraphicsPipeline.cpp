//
// Created by leixing on 2024/12/28.
//

#include "VulkanGraphicsPipeline.h"
#include "engine/Log.h"

namespace engine {
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VulkanDevice &vulkanDevice,
                                                   const VulkanSwapchain &swapchain,
                                                   const VulkanRenderPass &renderPass,
                                                   const vk::ShaderModule &vertexShaderModule,
                                                   const vk::ShaderModule &fragmentShaderModule,
                                                   const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                                                   const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                                                   const std::vector<vk::DescriptorSetLayout> & descriptorSetLayouts,
                                                   const std::vector<vk::PushConstantRange> & pushConstantRanges)
            : mDevice(vulkanDevice) {
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
        viewport.setX(0.0f)
                .setY(0.0f)
                .setWidth((float) displaySize.width)
                .setHeight((float) displaySize.height)
                .setMinDepth(0.0f)
                .setMaxDepth(1.0f);
        std::array<vk::Viewport, 1> viewports{viewport};

        vk::Rect2D scissor{};
        scissor.setOffset(vk::Offset2D{0, 0})
                .setExtent(displaySize);
        std::array<vk::Rect2D, 1> scissors{scissor};

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
        viewportStateCreateInfo
                .setViewports(viewports)
                .setScissors(scissors);

        // vertex shader
        vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
        vertexInputStateCreateInfo
                .setVertexBindingDescriptions(vertexInputBindingDescriptions)
                .setVertexAttributeDescriptions(vertexInputAttributeDescriptions);

        vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo;
        vertexShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eVertex)
                .setModule(vertexShaderModule)
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
                .setModule(fragmentShaderModule)
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
                .setPushConstantRanges(pushConstantRanges);

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
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
        LOG_D("VulkanGraphicsPipeline::~VulkanGraphicsPipeline");
        vk::Device device = mDevice.getDevice();
        device.destroy(mPipeline);
        device.destroy(mPipelineLayout);
    }

    const vk::Pipeline &VulkanGraphicsPipeline::getPipeline() const {
        return mPipeline;
    }

    const vk::PipelineLayout &VulkanGraphicsPipeline::getPipelineLayout() const {
        return mPipelineLayout;
    }
} // engine