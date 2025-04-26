//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "engine/vulkan_wrapper/pipeline/GraphicsPipeline.h"
#include "engine/vertex/configure/VertexConfigure.h"
#include "engine/configure/index_buffer/IndexBufferConfigure.h"
#include "engine/configure/push_constants/PushConstantConfigures.h"
#include "engine/configure/descriptor/PipelineLayoutConfigure.h"
#include "engine/vertex/configure/VertexBindingConfigure.h"

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
        VertexConfigure mVertexConfigure;

        /**
         * descriptor sets
         */
        PipelineLayoutConfigure mPipelineLayoutConfigure;

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
        GraphicsPipelineConfigure &addVertexBinding(const std::function<void(VertexBindingConfigure &)> &configure);

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
                                                const VulkanRenderPass &renderPass);
    };

} // engine
