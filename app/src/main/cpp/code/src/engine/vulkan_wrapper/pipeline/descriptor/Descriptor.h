//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <memory>

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/pipeline/resource/BufferInfo.h"
#include "engine/vulkan_wrapper/pipeline/resource/ImageInfo.h"
#include "engine/vulkan_wrapper/sampler/SamplerInterface.h"

namespace engine {

    class Descriptor {
    private:
        // uniform/sampler/storage/...
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

        std::vector<std::unique_ptr<SamplerInterface>> mImmutableSamplers;
        std::vector<vk::Sampler> mImmutableVkSamplers;
    public:

        Descriptor(vk::DescriptorType descriptorType,
                   uint32_t descriptorCount,
                   vk::ShaderStageFlags shaderStageFlags);

        Descriptor(vk::DescriptorType descriptorType,
                   std::vector<std::unique_ptr<SamplerInterface>> &&immutableSamplers,
                   vk::ShaderStageFlags shaderStageFlags);

        Descriptor(Descriptor &&other) noexcept;

        ~Descriptor();

        Descriptor& operator=(Descriptor &&other) noexcept;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorCount() const;

        [[nodiscard]]
        vk::ShaderStageFlags getShaderStageFlags() const;

        [[nodiscard]]
        const std::vector<vk::Sampler> &getImmutableVkSamplers() const;
    };

} // engine
