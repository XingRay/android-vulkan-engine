//
// Created by leixing on 2024/12/28.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanVertexShader.h"
#include "VulkanFragmentShader.h"
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
                            const std::vector<std::vector<std::unique_ptr<VulkanTextureSampler>>>& TextureSamplers);

        ~VulkanDescriptorSet();

        [[nodiscard]]
        const vk::DescriptorSetLayout &getDescriptorSetLayout() const;

        const vk::DescriptorPool &getDescriptorPool() const;

        const std::vector<vk::DescriptorSet> &getDescriptorSets() const;
    };

}
