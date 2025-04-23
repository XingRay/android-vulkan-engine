//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "engine/configure/vertex_buffer/VertexConfigures.h"
#include "engine/configure/index_buffer/IndexBufferConfigure.h"
#include "engine/configure/push_constants/PushConstantConfigures.h"
#include "engine/vulkan_wrapper/pipeline/GraphicsPipeline.h"
#include "engine/configure/descriptor/PipelineLayoutConfigure.h"

namespace engine {

    class VulkanEngineBuilder;

    class GraphicsPipelineConfigure {
        friend VulkanEngineBuilder;

    private:
        /**
         * shader code
         */
        std::vector<char> mVertexShaderCode;
        std::vector<char> mFragmentShaderCode;

        /**
         * vertex
         */
        VertexConfigures mVertexConfigures;

        /**
         * index
         */
        IndexBufferConfigure mVulkanIndexConfigure;

        /**
         * descriptor sets
         */
//        VulkanDescriptorSetConfigures mVulkanDescriptorSetConfigures;
        std::unique_ptr<PipelineLayoutConfigure> mPipelineLayoutConfigure;

        /**
         * push constant
         */
        PushConstantConfigures mPushConstantConfigures;

    public:
        explicit GraphicsPipelineConfigure();

        ~GraphicsPipelineConfigure();

        /**
         *
         * shader code
         *
         */
        GraphicsPipelineConfigure &vertexShaderCode(std::vector<char> &&code);

        GraphicsPipelineConfigure &fragmentShaderCode(std::vector<char> &&code);


        /**
         * vertex
         */
        GraphicsPipelineConfigure &addVertex(const std::function<void(VertexConfigure &)> &configure);

        /**
         * index
         */
        GraphicsPipelineConfigure &index(const std::function<void(IndexBufferConfigure &)> &configure);

        GraphicsPipelineConfigure &index(uint32_t capacity);

        GraphicsPipelineConfigure &index(uint32_t capacity, std::vector<uint32_t> &&indices);

        GraphicsPipelineConfigure &index(std::vector<uint32_t> &&indices);

        GraphicsPipelineConfigure &index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer);

        /**
         *
         * DescriptorSet
         *
         */
//        GraphicsPipelineConfigure &addUniformSet(const VulkanDescriptorSet &uniformSet);

//        GraphicsPipelineConfigure &addDescriptorSet(const std::function<void(DescriptorSetConfigure &)> &configure);


        /**
         * push constant
         */
        GraphicsPipelineConfigure &addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits);

    private:
        [[nodiscard]]
        std::unique_ptr<GraphicsPipeline> build(const VulkanDevice &vulkanDevice,
                                                const VulkanSwapchain &swapchain,
                                                const VulkanRenderPass &renderPass,
                                                const VulkanCommandPool &commandPool,
                                                uint32_t frameCount) const;
    };

} // engine
