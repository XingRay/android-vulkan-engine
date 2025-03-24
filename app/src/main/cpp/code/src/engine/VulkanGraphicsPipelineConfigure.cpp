//
// Created by leixing on 2025/3/7.
//

#include "VulkanGraphicsPipelineConfigure.h"
#include "engine/vulkan_wrapper/VulkanShaderModule.h"
#include "engine/vulkan_wrapper/VulkanDescriptorPool.h"

namespace engine {

    VulkanGraphicsPipelineConfigure::VulkanGraphicsPipelineConfigure() = default;

    VulkanGraphicsPipelineConfigure::~VulkanGraphicsPipelineConfigure() = default;


    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::vertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::fragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::addVertex(const std::function<void(VulkanVertexConfigure &)> &configure) {
        VulkanVertexConfigure config{};
        configure(config);
        mVulkanVertexConfigures.addVulkanVertexConfigure(config);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::index(const std::function<void(VulkanIndexConfigure &)> &configure) {
        VulkanIndexConfigure config{};
        configure(config);
        mVulkanIndexConfigure = config;
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::index(uint32_t capacity) {
        mVulkanIndexConfigure.setIndexBuffer(capacity);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::index(uint32_t capacity, std::vector<uint32_t> &&indices) {
        mVulkanIndexConfigure.setIndexBuffer(capacity, std::move(indices));
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::index(std::vector<uint32_t> &&indices) {
        mVulkanIndexConfigure.setIndexBuffer(std::move(indices));
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer) {
        mVulkanIndexConfigure.setIndexBuffer(indexBuffer);
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::addDescriptorSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure) {
        std::unique_ptr<VulkanDescriptorSetConfigure> config = std::make_unique<VulkanDescriptorSetConfigure>();
        configure(*config);
        mVulkanDescriptorSetConfigures.addVulkanDescriptorSetConfigure(std::move(config));
        return *this;
    }

    VulkanGraphicsPipelineConfigure &VulkanGraphicsPipelineConfigure::addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits) {
        mVulkanPushConstantConfigures.addVulkanPushConstantConfigures(VulkanPushConstantConfigure(size, offset, stageFlagBits));
        return *this;
    }

    std::unique_ptr<VulkanGraphicsPipeline> VulkanGraphicsPipelineConfigure::build(const VulkanDevice &vulkanDevice,
                                                                                   const VulkanSwapchain &swapchain,
                                                                                   const VulkanRenderPass &renderPass,
                                                                                   const VulkanCommandPool &commandPool,
                                                                                   uint32_t frameCount) const {

        // shader code
        std::unique_ptr<VulkanShaderModule> vertexShaderModule = std::make_unique<VulkanShaderModule>(vulkanDevice, mVertexShaderCode);
        std::unique_ptr<VulkanShaderModule> fragmentShaderModule = std::make_unique<VulkanShaderModule>(vulkanDevice, mFragmentShaderCode);

        // vertex buffer
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = mVulkanVertexConfigures.createVertexInputBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = mVulkanVertexConfigures.createVertexInputAttributeDescriptions();
        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> vertexBuffers = mVulkanVertexConfigures.createVertexBuffers(vulkanDevice, commandPool);

        // index buffer
        // todo index buffers
        std::shared_ptr<VulkanDeviceLocalIndexBuffer> indexBuffer = mVulkanIndexConfigure.createVertexBuffer(vulkanDevice, commandPool);

        // descriptor -> uniform / sampler ...
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = mVulkanDescriptorSetConfigures.createDescriptorSetLayouts(vulkanDevice);
        std::unique_ptr<VulkanDescriptorPool> vulkanDescriptorPool = std::make_unique<VulkanDescriptorPool>(vulkanDevice, mVulkanDescriptorSetConfigures.createDescriptorPoolSizes(frameCount),
                                                                                                            mVulkanDescriptorSetConfigures.getSetCount(frameCount));


        // push constant
        std::vector<vk::PushConstantRange> pushConstantRanges = mVulkanPushConstantConfigures.createPushConstantRanges();
        // 检查总大小是否超出设备限制
        uint32_t maxPushConstantsSize = vulkanDevice.getMaxPushConstantsSize();
        uint32_t totalPushConstantSize = mVulkanPushConstantConfigures.calcTotalPushConstantSize();
        if (totalPushConstantSize > maxPushConstantsSize) {
            throw std::runtime_error(
                    "Total Push Constant size (" + std::to_string(totalPushConstantSize) +
                    ") exceeds device limit: " + std::to_string(maxPushConstantsSize)
            );
        }

        return std::make_unique<VulkanGraphicsPipeline>(vulkanDevice, swapchain, renderPass,
                                                        *vertexShaderModule, *fragmentShaderModule,
                                                        vertexInputBindingDescriptions,
                                                        vertexInputAttributeDescriptions,
                                                        std::move(vertexBuffers),
                                                        indexBuffer,
                                                        frameCount,
                                                        std::move(vulkanDescriptorPool),
                                                        descriptorSetLayouts,
                                                        std::move(pushConstantRanges));
    }

} // engine