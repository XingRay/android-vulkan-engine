//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDescriptorBinding/VulkanDescriptorBinding.h"
#include "engine/vulkan_wrapper/VulkanBufferView.h"

namespace engine {

    class VulkanBufferDescriptorBinding : public VulkanDescriptorBinding {
    private:
        // buffer view
        std::unique_ptr<VulkanBufferView> mVulkanBufferView;

        //descriptor view
        vk::DescriptorType mDescriptorType;
        uint32_t mSet;
        uint32_t mBinding;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

    public:
        VulkanBufferDescriptorBinding();

        ~VulkanBufferDescriptorBinding();

        [[nodiscard]]
        vk::WriteDescriptorSet createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet) const override;
    };

} // engine
