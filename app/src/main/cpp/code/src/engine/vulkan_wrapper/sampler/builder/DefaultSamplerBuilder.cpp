//
// Created by leixing on 2025/4/11.
//

#include "DefaultSamplerBuilder.h"
#include "engine/vulkan_wrapper/sampler/DefaultSampler.h"

namespace engine {

    DefaultSamplerBuilder::DefaultSamplerBuilder()
            : mMaxLoad(0.0f) {};

    DefaultSamplerBuilder::~DefaultSamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> DefaultSamplerBuilder::build(const VulkanDevice &vulkanDevice) {
        return std::make_unique<DefaultSampler>(vulkanDevice, mMaxLoad);
    }

    DefaultSamplerBuilder &DefaultSamplerBuilder::setMaxLoad(float maxLoad) {
        mMaxLoad = maxLoad;
        return *this;
    }

} // engine