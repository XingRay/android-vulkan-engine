//
// Created by leixing on 2025/3/7.
//

#include "engine/configure/GraphicsPipelineConfigure.h"
#include "engine/vulkan_wrapper/VulkanShaderModule.h"
//#include "engine/vulkan_wrapper/VulkanDescriptorPool.h"
#include "engine/vulkan_wrapper/pipeline/descriptor/VulkanDescriptorBindingSets.h"

namespace engine {

    GraphicsPipelineConfigure::GraphicsPipelineConfigure() = default;

    GraphicsPipelineConfigure::~GraphicsPipelineConfigure() = default;


    GraphicsPipelineConfigure &GraphicsPipelineConfigure::vertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::fragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::addVertex(const std::function<void(VertexConfigure &)> &configure) {
        VertexConfigure config{};
        configure(config);
        mVulkanVertexConfigures.addVertexConfigure(config);
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::index(const std::function<void(IndexBufferConfigure &)> &configure) {
        IndexBufferConfigure config{};
        configure(config);
        mVulkanIndexConfigure = config;
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::index(uint32_t capacity) {
        mVulkanIndexConfigure.setIndexBuffer(capacity);
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::index(uint32_t capacity, std::vector<uint32_t> &&indices) {
        mVulkanIndexConfigure.setIndexBuffer(capacity, std::move(indices));
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::index(std::vector<uint32_t> &&indices) {
        mVulkanIndexConfigure.setIndexBuffer(std::move(indices));
        return *this;
    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer) {
        mVulkanIndexConfigure.setIndexBuffer(indexBuffer);
        return *this;
    }

//    GraphicsPipelineConfigure &GraphicsPipelineConfigure::addDescriptorSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure) {
//        std::unique_ptr<VulkanDescriptorSetConfigure> config = std::make_unique<VulkanDescriptorSetConfigure>();
//        configure(*config);
//        mVulkanDescriptorSetConfigures.addVulkanDescriptorSetConfigure(std::move(config));
//        return *this;
//    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits) {
        mVulkanPushConstantConfigures.addVulkanPushConstantConfigures(VulkanPushConstantConfigure(size, offset, stageFlagBits));
        return *this;
    }

    std::unique_ptr<GraphicsPipeline> GraphicsPipelineConfigure::build(const VulkanDevice &vulkanDevice,
                                                                       const VulkanSwapchain &swapchain,
                                                                       const VulkanRenderPass &renderPass,
                                                                       const VulkanCommandPool &commandPool,
                                                                       uint32_t frameCount) const {

        // shader code
        VulkanShaderModule vertexShaderModule = VulkanShaderModule(vulkanDevice, mVertexShaderCode);
        VulkanShaderModule fragmentShaderModule = VulkanShaderModule(vulkanDevice, mFragmentShaderCode);

        // vertex buffer
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = mVulkanVertexConfigures.createVertexInputBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = mVulkanVertexConfigures.createVertexInputAttributeDescriptions();
        std::vector<std::shared_ptr<VulkanDeviceLocalVertexBuffer>> vertexBuffers = mVulkanVertexConfigures.createVertexBuffers(vulkanDevice, commandPool);

        // index buffer
        // todo support index buffers
        std::shared_ptr<VulkanDeviceLocalIndexBuffer> indexBuffer = mVulkanIndexConfigure.createVertexBuffer(vulkanDevice, commandPool);

        // descriptor -> uniform / sampler ...
//        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = mVulkanDescriptorSetConfigures.createDescriptorSetLayouts(vulkanDevice);

        //        std::unique_ptr<VulkanDescriptorPool> vulkanDescriptorPool = std::make_unique<VulkanDescriptorPool>(vulkanDevice,
//                                                                                                            mVulkanDescriptorSetConfigures.createDescriptorPoolSizes(frameCount),
//                                                                                                            mVulkanDescriptorSetConfigures.getSetCount(frameCount));

        std::unique_ptr<PipelineLayout> pipelineLayout = mPipelineLayoutConfigure->createPipelineLayout(vulkanDevice);

        // frame -> set -> binding
        std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> vulkanDescriptorBindingSets;
        for (uint32_t frameIndex = 0; frameIndex < frameCount; frameIndex++) {
//            std::unique_ptr<VulkanDescriptorBindingSets> bindingSetsOfFrame = mVulkanDescriptorSetConfigures.createVulkanDescriptorBindingSets(vulkanDevice, commandPool);
//            vulkanDescriptorBindingSets.push_back(std::move(bindingSetsOfFrame));
        }

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

        return std::make_unique<GraphicsPipeline>(vulkanDevice,
                                                  swapchain,
                                                  renderPass,
                                                  vertexShaderModule,
                                                  fragmentShaderModule,
                                                  vertexInputBindingDescriptions,
                                                  vertexInputAttributeDescriptions,
                                                  std::move(vertexBuffers),
                                                  indexBuffer,
                                                  std::move(pipelineLayout),
                                                  frameCount
//                                                  std::move(vulkanDescriptorPool),
//                                                  descriptorSetLayouts,
                                                  /*std::move(vulkanDescriptorBindingSets),
                                                  std::move(pushConstantRanges)*/);
    }

} // engine