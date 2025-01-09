//
// Created by leixing on 2024/12/28.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanSwapchain.h"
#include "engine/vulkan_wrapper/VulkanVertexShader.h"
#include "engine/vulkan_wrapper/VulkanFragmentShader.h"
//#include "VulkanUniformBuffer.h"

namespace engine {

    class VulkanUniformBuffer;
    class VulkanTextureSampler;

    class VulkanDescriptorSet {
    private:
        const VulkanDevice &mDevice;

        vk::DescriptorSetLayout mDescriptorSetLayout;
        vk::DescriptorPool mDescriptorPool;
        std::vector<vk::DescriptorSet> mDescriptorSets;

    public:
        VulkanDescriptorSet(const VulkanDevice &device,
                            uint32_t frameCount,
                            const VulkanVertexShader &vertexShader,
                            const VulkanFragmentShader &fragmentShader,
                            const std::vector<std::vector<std::unique_ptr<VulkanUniformBuffer>>>& uniformBuffers,
                            const std::vector<std::vector<std::unique_ptr<VulkanTextureSampler>>>& textureSamplers);

        ~VulkanDescriptorSet();

        [[nodiscard]]
        const vk::DescriptorSetLayout &getDescriptorSetLayout() const;

        const vk::DescriptorPool &getDescriptorPool() const;

        const std::vector<vk::DescriptorSet> &getDescriptorSets() const;
    };

}
