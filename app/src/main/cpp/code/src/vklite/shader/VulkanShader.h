//
// Created by leixing on 2025/1/5.
//

#pragma once

//#include "vulkan/vulkan.hpp"
//
//#include "vklite/vulkan_wrapper/VulkanInstance.h"
//#include "vklite/vulkan_wrapper/VulkanDevice.h"
//#include "vklite/vulkan_wrapper/VulkanCommandPool.h"
//#include "vklite/ShaderFormat.h"
//#include "vklite/ImageSize.h"
//#include "vklite/vulkan_wrapper/buffer/VulkanHostVisibleBuffer.h"
//#include "vklite/VulkanPushConstantConfigure.h"
//#include "vklite/vulkan_wrapper/buffer/VulkanSampler.h"

namespace vklite {
//    class VulkanCommandPool;
//
//    class VulkanShader {
//    private:
//        const VulkanDevice &mVulkanDevice;
//
//        // uniform buffer/ texture sampler / storage buffer
//        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;
//
//        // uniform set -> binding -> ImmutableSamplers
//        std::vector<std::vector<std::vector<std::unique_ptr<VulkanSampler>>>> mImmutableSamplers;
//        std::vector<std::vector<std::vector<vk::Sampler>>> mSamplers;
//
//        vk::DescriptorPool mDescriptorPool;
//
//        // frame -> uniform set -> binding
//        std::vector<std::vector<std::vector<std::unique_ptr<VulkanBuffer>>>> mBuffers;
//
//        // push constant
//        std::vector<vk::PushConstantRange> mPushConstantRanges;
//        std::vector<std::vector<uint8_t>> mPushConstantDataList;
//
//    public:
//        explicit VulkanShader(const VulkanInstance &vulkanInstance,
//                              const VulkanDevice &vulkanDevice,
//                              const VulkanCommandPool &commandPool,
//                              uint32_t frameCount,
////                              const std::vector<VulkanVertex> &vertices,
////                              const std::vector<VulkanDescriptorSet> &descriptorSets,
//                              const std::vector<VulkanPushConstantConfigure> &pushConstants);
//
//        ~VulkanShader();
//
//        [[nodiscard]]
//        const vk::ShaderModule &getComputeShaderModule() const;
//
//        [[nodiscard]]
//        const vk::ShaderModule &getVertexShaderModule() const;
//
//        [[nodiscard]]
//        const vk::ShaderModule &getFragmentShaderModule() const;
//
//        [[nodiscard]]
//        const std::vector<vk::VertexInputBindingDescription> &getVertexDescriptions() const;
//
//        [[nodiscard]]
//        const std::vector<vk::VertexInputAttributeDescription> &getVertexInputAttributeDescriptions() const;
//
//        [[nodiscard]]
//        const std::vector<vk::DescriptorSetLayout> &getDescriptorSetLayouts() const;
//
//        [[nodiscard]]
//        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;
//
//
//        [[nodiscard]]
//        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;
//
//        [[nodiscard]]
//        const std::vector<std::vector<uint8_t>> &getPushConstantDataList() const;
//
//        void updateBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, void *data, uint32_t size);
//
//        void updatePushConstant(uint32_t index, const void *data);
//    };

} // engine
