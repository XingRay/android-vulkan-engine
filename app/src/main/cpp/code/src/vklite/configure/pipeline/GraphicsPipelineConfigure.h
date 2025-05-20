//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "vklite/pipeline/GraphicsPipeline.h"
#include "vklite/vertex/configure/VertexConfigure.h"
#include "vklite/configure/index_buffer/IndexBufferConfigure.h"
#include "vklite/configure/push_constants/PushConstantConfigures.h"
#include "vklite/configure/descriptor/PipelineLayoutConfigure.h"
#include "vklite/vertex/configure/VertexBindingConfigure.h"

namespace vklite {

    class VkLiteEngineBuilder;

    class GraphicsPipelineConfigure {
        friend VkLiteEngineBuilder;

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
