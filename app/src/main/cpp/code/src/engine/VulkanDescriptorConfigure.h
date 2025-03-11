//
// Created by leixing on 2025/1/14.
//

#pragma once

//#include <variant>
#include <memory>
#include "vulkan/vulkan.hpp"

//#include "engine/ImageSize.h"

//#include <android/hardware_buffer.h>
#include "engine/vulkan_wrapper/VulkanSampler.h"

namespace engine {

//    enum class VulkanDescriptorType {
//        uniform,
//        sampler,
//        androidHardwareBufferSampler,
//        storage,
//    };
//
//    class VulkanUniformData {
//    public:
//        uint32_t size;
//    };
//
//    class VulkanSamplerData {
//    public:
//        ImageSize imageSize;
//    };
//
//    class VulkanAndroidHardwareBufferSamplerData {
//    public:
//        AHardwareBuffer *hardwareBuffer;
//    };

    class VulkanDescriptorConfigure {
    private:
//        VulkanDescriptorType mVulkanDescriptorType;
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;
//        uint32_t mIndex;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlagBits mShaderStageFlagBits;
        std::vector<std::unique_ptr<VulkanSampler>> mImmutableSamplers;

//        std::variant<VulkanUniformData, VulkanSamplerData, VulkanAndroidHardwareBufferSamplerData> mData;
    public:

        VulkanDescriptorConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlagBits shaderStageFlagBits,
                                  std::vector<std::unique_ptr<VulkanSampler>> &&immutableSamplers);

        VulkanDescriptorConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorCount, vk::ShaderStageFlagBits shaderStageFlagBits);

        ~VulkanDescriptorConfigure();

//        [[nodiscard]]
//        VulkanDescriptorType getVulkanDescriptorType() const;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

//        uint32_t getIndex() const;

        uint32_t getDescriptorCount() const;

        vk::ShaderStageFlagBits getStageFlags() const;

        const std::vector<std::unique_ptr<VulkanSampler>> &getImmutableSamplers() const;

//        const VulkanUniformData &getUniformData() const;
//
//        const VulkanSamplerData &getSamplerData() const;
//
//        const VulkanAndroidHardwareBufferSamplerData &getVulkanAndroidHardwareBufferSamplerData() const;
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding();
    };

//    class VulkanUniformDescriptor : public VulkanDescriptor {
//
//    public:
//        explicit VulkanUniformDescriptor(uint32_t binding);
//
//        ~VulkanUniformDescriptor() override;
//    };
//
//    class VulkanSamplerDescriptor : public VulkanDescriptor {
//
//    public:
//
//        explicit VulkanSamplerDescriptor(uint32_t binding);
//
//        ~VulkanSamplerDescriptor() override;
//    };

} // engine
