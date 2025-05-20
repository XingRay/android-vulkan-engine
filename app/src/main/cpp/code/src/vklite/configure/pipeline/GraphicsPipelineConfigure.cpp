//
// Created by leixing on 2025/3/7.
//

#include "GraphicsPipelineConfigure.h"
#include "vklite/shader/VulkanShaderModule.h"
#include "vklite/pipeline/descriptor/VulkanDescriptorBindingSets.h"

namespace vklite {

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

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::addVertexBinding(const std::function<void(VertexBindingConfigure &)> &configure) {
        VertexBindingConfigure config{};
        configure(config);
        mVertexConfigure.add(std::move(config));
        return *this;
    }

    //    GraphicsPipelineConfigure &GraphicsPipelineConfigure::addDescriptorSet(const std::function<void(VulkanDescriptorSetConfigure &)> &configure) {
//        std::unique_ptr<VulkanDescriptorSetConfigure> config = std::make_unique<VulkanDescriptorSetConfigure>();
//        configure(*config);
//        mVulkanDescriptorSetConfigures.addVulkanDescriptorSetConfigure(std::move(config));
//        return *this;
//    }

    GraphicsPipelineConfigure &GraphicsPipelineConfigure::addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits) {
//        mPushConstantConfigures.addPushConstantConfigures(PushConstantConfigure(size, offset, stageFlagBits));
        return *this;
    }

    std::unique_ptr<GraphicsPipeline> GraphicsPipelineConfigure::build(const VulkanDevice &vulkanDevice,
                                                                       const VulkanSwapchain &swapchain,
                                                                       const VulkanRenderPass &renderPass) {

        // shader code
        VulkanShaderModule vertexShaderModule = VulkanShaderModule(vulkanDevice, mVertexShaderCode);
        VulkanShaderModule fragmentShaderModule = VulkanShaderModule(vulkanDevice, mFragmentShaderCode);

        // vertex buffer description
        std::unique_ptr<VertexBufferDescription> vertexBufferDescription = mVertexConfigure.createVertexBufferDescription();

        // descriptor -> uniform / sampler ...
//        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = mVulkanDescriptorSetConfigures.createDescriptorSetLayouts(vulkanDevice);

        //        std::unique_ptr<VulkanDescriptorPool> vulkanDescriptorPool = std::make_unique<VulkanDescriptorPool>(vulkanDevice,
//                                                                                                            mVulkanDescriptorSetConfigures.createDescriptorPoolSizes(frameCount),
//                                                                                                            mVulkanDescriptorSetConfigures.getSetCount(frameCount));

        std::unique_ptr<PipelineLayout> pipelineLayout = mPipelineLayoutConfigure.createPipelineLayout(vulkanDevice);

        // frame -> set -> binding
//        std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> vulkanDescriptorBindingSets;
//        for (uint32_t frameIndex = 0; frameIndex < frameCount; frameIndex++) {
//            std::unique_ptr<VulkanDescriptorBindingSets> bindingSetsOfFrame = mVulkanDescriptorSetConfigures.createVulkanDescriptorBindingSets(vulkanDevice, commandPool);
//            vulkanDescriptorBindingSets.push_back(std::move(bindingSetsOfFrame));
//        }

//        // push constant
//        std::vector<vk::PushConstantRange> pushConstantRanges = mPushConstantConfigures.createPushConstantRanges();
//        // 检查总大小是否超出设备限制
//        uint32_t maxPushConstantsSize = vulkanDevice.getMaxPushConstantsSize();
//        uint32_t totalPushConstantSize = pipelineLayout->calcTotalPushConstantSize();
//        if (totalPushConstantSize > maxPushConstantsSize) {
//            throw std::runtime_error(
//                    "Total Push Constant size (" + std::to_string(totalPushConstantSize) +
//                    ") exceeds device limit: " + std::to_string(maxPushConstantsSize)
//            );
//        }

        return std::make_unique<GraphicsPipeline>(vulkanDevice,
                                                  swapchain,
                                                  renderPass,
                                                  vertexShaderModule,
                                                  fragmentShaderModule,
                                                  *vertexBufferDescription,
                                                  std::move(pipelineLayout));
    }

} // engine