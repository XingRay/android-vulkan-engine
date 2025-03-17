//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "engine/VulkanVertexConfigures.h"
#include "engine/VulkanDescriptorSetConfigures.h"
#include "engine/VulkanPushConstantConfigures.h"

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
        VulkanVertexConfigures mVulkanVertexConfigures;

        /**
         * descriptor sets
         */
        VulkanDescriptorSetConfigures mVulkanDescriptorSetConfigures;

        /**
         * push constant
         */
        VulkanPushConstantConfigures mVulkanPushConstantConfigures;

    public:
        explicit VulkanGraphicsPipelineConfigure();

        ~VulkanGraphicsPipelineConfigure();

        /**
         *
         * shader code
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
