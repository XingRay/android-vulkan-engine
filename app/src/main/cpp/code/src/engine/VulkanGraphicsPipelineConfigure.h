//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "engine/VulkanEngineBuilder.h"
#include "engine/VulkanVertexConfigure.h"
#include "engine/VulkanDescriptorSetConfigure.h"

namespace engine {

    class VulkanGraphicsPipelineConfigure {
    private:
        /**
         * shader code
         */
        std::vector<char> mVertexShaderCode;
        std::vector<char> mFragmentShaderCode;

        /**
         * vertex
         */
        std::vector<VulkanVertexConfigure> mVulkanVertexConfigures;

        /**
         * uniform
         */
        std::vector<VulkanDescriptorSetConfigure> mVulkanDescriptorSetConfigures;

        /**
         * push constant
         */
        std::vector<VulkanPushConstant> mPushConstants;

    public:
        explicit VulkanGraphicsPipelineConfigure();

        ~VulkanGraphicsPipelineConfigure();

        /**
         *
         * shader coce
         *
         */
        VulkanGraphicsPipelineConfigure &vertexShaderCode(std::vector<char> &&code);

        VulkanGraphicsPipelineConfigure &fragmentShaderCode(std::vector<char> &&code);


        /**
         * vertex
         */
        VulkanGraphicsPipelineConfigure &vertex(const std::function<void(VulkanVertexConfigure &)> &configure);

//        VulkanGraphicsPipelineConfigure &addVertex(const VulkanVertex &vertex);


        /**
         *
         * DescriptorSet
         *
         */
//        VulkanGraphicsPipelineConfigure &addUniformSet(const VulkanDescriptorSet &uniformSet);

        VulkanGraphicsPipelineConfigure &uniformSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure);


        /**
         * push constant
         */
        VulkanGraphicsPipelineConfigure &addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits);

        std::unique_ptr<VulkanGraphicsPipeline> build(const VulkanDevice &vulkanDevice,
                                                      const VulkanSwapchain &swapchain,
                                                      const VulkanRenderPass &renderPass);
    };

} // engine
