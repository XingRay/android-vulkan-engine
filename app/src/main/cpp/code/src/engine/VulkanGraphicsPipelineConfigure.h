//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "engine/VulkanVertexConfigures.h"
#include "engine/VulkanIndexConfigure.h"
#include "engine/VulkanDescriptorSetConfigures.h"
#include "engine/VulkanPushConstantConfigures.h"
#include "engine/vulkan_wrapper/VulkanGraphicsPipeline.h"

namespace engine {

    class VulkanEngineBuilder;

    class VulkanGraphicsPipelineConfigure {
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
        VulkanVertexConfigures mVulkanVertexConfigures;

        /**
         * index
         */
        VulkanIndexConfigure mVulkanIndexConfigure;

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
        VulkanGraphicsPipelineConfigure &addVertex(const std::function<void(VulkanVertexConfigure &)> &configure);

        /**
         * index
         */
        VulkanGraphicsPipelineConfigure &index(const std::function<void(VulkanIndexConfigure &)> &configure);

        VulkanGraphicsPipelineConfigure &index(uint32_t capacity);

        VulkanGraphicsPipelineConfigure &index(uint32_t capacity, std::vector<uint32_t> &&indices);

        VulkanGraphicsPipelineConfigure &index(std::vector<uint32_t> &&indices);

        VulkanGraphicsPipelineConfigure &index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer);

        /**
         *
         * DescriptorSet
         *
         */
//        VulkanGraphicsPipelineConfigure &addUniformSet(const VulkanDescriptorSet &uniformSet);

        VulkanGraphicsPipelineConfigure &addDescriptorSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure);


        /**
         * push constant
         */
        VulkanGraphicsPipelineConfigure &addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits);

    private:
        [[nodiscard]]
        std::unique_ptr<VulkanGraphicsPipeline> build(const VulkanDevice &vulkanDevice,
                                                      const VulkanSwapchain &swapchain,
                                                      const VulkanRenderPass &renderPass,
                                                      const VulkanCommandPool &commandPool,
                                                      uint32_t frameCount) const;
    };

} // engine
